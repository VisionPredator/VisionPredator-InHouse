#include "pch.h"
#include <memory>

#include "ParticleObject.h"
#include "Util.h"

#include "Device.h"
#include "BlendState.h"
#include "D3DUtill.h"
#include "Defines.h"
#include "DepthStencilState.h"
#include "VertexBuffer.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

ParticleObject::ParticleObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const effect::ParticleInfo& info)
	: m_Device(device), m_ResourceManager(resourceManager), m_Info(info)
{
#pragma region �ӽ�
	m_Info.MaxParticles = 500;
#pragma endregion

	// Create Buffers
	{
		ParticleVertex p;
		ZeroMemory(&p, sizeof(ParticleVertex));
		p.Age = 0.0f;
		p.Type = 0;

		D3D11_BUFFER_DESC vbd;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.ByteWidth = sizeof(ParticleVertex) * 1;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA vinitData = {};
		vinitData.pSysMem = &p;
		
		HR_CHECK(device->Get()->CreateBuffer(&vbd, &vinitData, &m_InitVB));			// ��Ʈ�� ��¿� ����

		vbd.ByteWidth = sizeof(ParticleVertex) * m_Info.MaxParticles; // �ִ�� �ٷ� ���� ũ��. �Ѿ�� �ȵȴ�.
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; // �÷��� �߿�!
		HR_CHECK(device->Get()->CreateBuffer(&vbd, nullptr, &m_DrawVB));			// 
		HR_CHECK(device->Get()->CreateBuffer(&vbd, nullptr, &m_StreamOutVB));

		m_FrameCB = m_ResourceManager->Create<ConstantBuffer<PerFrame>>(L"FrameCB").lock();

		D3D11_BUFFER_DESC cbd;
		cbd.Usage = D3D11_USAGE_DEFAULT;
		static_assert(sizeof(PerFrame) % 16 == 0, "must be align");
		cbd.ByteWidth = sizeof(PerFrame);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = 0;
		cbd.MiscFlags = 0;

		HR_CHECK(device->Get()->CreateBuffer(&cbd, NULL, m_FrameCB->GetAddress()));
	}

	// Create Shaders
	{
		m_StreamOutVS = std::make_shared<VertexShader>(device, L"ParticleVS", "StreamOutVS");
		m_DrawVS = std::make_shared<VertexShader>(device, L"ParticleVS", "DrawVS");
		m_StreamOutGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "StreamOutGS", "gs_5_0");
		m_DrawGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "DrawGS", "gs_5_0");
		m_DrawPS = std::make_shared<PixelShader>(device, L"ParticlePS", "DrawPS");

		m_SamLinear = m_ResourceManager->Get<Sampler>(L"Linear").lock();
	}

	// TODO: SRV �̿��ϴ� �ɷ� �ٲ�� �Ѵ�.
	// Create Textures
	{
		std::vector<std::wstring> flares;
		if (!m_Info.TexturePath.empty())
		{
			flares.push_back(Util::ToWideChar(m_Info.TexturePath));
		}
		else
		{
			flares.push_back(Util::ToWideChar("../../../Resource/Texture/flare0.dds"));	
		}
		D3DUtill::CreateTexture2DArraySRV(device->Get(), device->Context(), m_TexArraySRV.GetAddressOf(), flares);

		if (m_Info.TexturePath.empty())
			m_Info.TexturePath = "../../../Resource/Texture/flare0.dds";
		m_TextureSRV = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.TexturePath).c_str(), Util::ToWideChar(m_Info.TexturePath).c_str()).lock();
		
		// ���� �ؽ�ó ����
		m_RandomTextureSRV = m_ResourceManager->Create<ShaderResourceView>(L"RandomTextureSRV").lock();
		D3DUtill::CreateRandomTexture1DSRV(device->Get(), m_RandomTexSRV.GetAddressOf());
		D3DUtill::CreateRandomTexture1DSRV(device->Get(), m_RandomTextureSRV->GetAddress());
	}
}

void ParticleObject::Update(const float& deltaTime, const float& totalGameTime)
{
	m_TotalGameTime = totalGameTime;
	m_TimeStep = deltaTime;
	m_Age += deltaTime;

	if (m_Info.TexturePath.empty())
		m_Info.TexturePath = "../../../Resource/Texture/flare0.dds";
	m_TextureSRV = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.TexturePath).c_str(), Util::ToWideChar(m_Info.TexturePath).c_str()).lock();
}

void ParticleObject::Draw()
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = m_Device->Context();
	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock();

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	Matrix viewInvert = CameraCB->m_struct.view.Invert().Transpose();
	Matrix VP = CameraCB->m_struct.view * CameraCB->m_struct.proj;

	Matrix VPTTT = CameraCB->m_struct.view.Transpose() * CameraCB->m_struct.proj.Transpose();
	VPTTT = VPTTT.Transpose();

	// CB data ����
	m_PerFrame.ViewProj = VPTTT;
	m_PerFrame.GameTime = m_TotalGameTime;
	m_PerFrame.TimeStep = m_TimeStep;
	m_PerFrame.EyePosW = viewInvert.Translation();
	m_PerFrame.EmitPosW = m_EmitPosW;
	m_PerFrame.EmitDirW = m_EmitDirW;

	m_FrameCB->Update(m_PerFrame);

	// Bind
	context->IASetInputLayout(m_DrawVS->InputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	if (m_FirstRun)	// ó�� �׸� �� �ʱ�ȭ ���۷� �׸���.
		context->IASetVertexBuffers(0, 1, m_InitVB.GetAddressOf(), &stride, &offset);
	else
		context->IASetVertexBuffers(0, 1, m_DrawVB.GetAddressOf(), &stride, &offset);

	// Bind Shader's resources
	context->VSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->VSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->VSSetShaderResources(0, 1, m_TexArraySRV.GetAddressOf());
	context->VSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->GSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->GSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->GSSetShaderResources(0, 1, m_TexArraySRV.GetAddressOf());
	context->GSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->PSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->PSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->PSSetShaderResources(0, 1, m_TexArraySRV.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->VSSetShader(m_StreamOutVS->GetVS(), nullptr, 0);
	context->GSSetShader(m_StreamOutGS->GetShader(), nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);	// PS ��Ȱ��ȭ

	// �ε��� ���� ���� ����
	// ���� ���� ��Ȱ��ȭ
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	context->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"DisableDepth").lock()->GetState().Get(), 0);

	/// 1. ��Ʈ�� �ƿ����� ���� ���� �� ���� ���� ��ƼŬ ����
	context->SOSetTargets(1,			// ���� ���� ����, �ִ� 4��
		m_StreamOutVB.GetAddressOf(),	// ��¿� ����
		&offset);						// ���� ���۸��� ����ϱ� ������ ��ġ�� ��Ÿ����. ���� 0

	if (m_FirstRun)
	{
		context->Draw(1, 0);
		m_FirstRun = false;
	}
	else
	{
		context->DrawAuto();	// ���������� GPU�� �˾Ƽ� ���� ������ŭ draw �� ���ش�.
	}

	/// 2. ������ ������� ���� Draw �� ����
	ID3D11Buffer* bufferArray[1] = { nullptr };
	context->OMSetDepthStencilState(nullptr, 0);	// ����ε�

	context->SOSetTargets(1, bufferArray, &offset);	// ��� �ܰ踦 ����ش�.
	std::swap(m_DrawVB, m_StreamOutVB);	// ������ ���� swap �Ѵ�.

	// ȭ�� ������
	context->VSSetShader(m_DrawVS->GetShader(), nullptr, 0);
	context->GSSetShader(m_DrawGS->GetShader(), nullptr, 0);
	context->PSSetShader(m_DrawPS->GetShader(), nullptr, 0);

	float factor[] = { 0.f, 0.f, 0.f, 0.f };
	context->OMSetBlendState(m_ResourceManager->Get<BlendState>(L"AdditiveBlending").lock()->GetState().Get(), factor, 0xffffffff);	// ���� ȥ��
	context->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"NoDepthWrites").lock()->GetState().Get(), 0);	// ���� ����� ���� �ʴ´�.
	context->IASetVertexBuffers(0, 1, m_DrawVB.GetAddressOf(), &stride, &offset);
	context->DrawAuto();

	// ����ε�
	context->OMSetBlendState(nullptr, factor, 0xffffffff);
	context->OMSetDepthStencilState(nullptr, 0);

	// �⺻ ������ �缳��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
