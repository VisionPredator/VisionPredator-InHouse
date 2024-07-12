#include "pch.h"
#include "ParticleObject.h"

#include <memory>
#include <memory>
#include <memory>

#include "BlendState.h"
#include "D3DUtill.h"
#include "Defines.h"
#include "DepthStencilState.h"
#include "Device.h"
#include "VertexBuffer.h"
#include "ResourceManager.h"
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"

ParticleObject::ParticleObject(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager)
{
	// Temp Data.
	// TODO: �������̽� ���� �ְ���� �� �ִ� �������� �ٲٱ�
	{
		m_MaxParticles = 500;
	}

	m_ResourceManager = resourceManager;

	m_StreamOutVS = std::make_shared<VertexShader>(device, L"ParticleVS", "DrawVS");
	m_DrawVS = std::make_shared<VertexShader>(device, L"ParticleVS", "DrawVS");

	m_StreamOutGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "StreamOutGS", "gs_5_0");
	m_DrawGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "DrawGS", "gs_5_0");

	m_DrawPS = std::make_shared<PixelShader>(device, L"ParticlePS");

	D3D11_BUFFER_DESC cbd;
	cbd.Usage = D3D11_USAGE_DEFAULT;
	static_assert(sizeof(PerFrame) % 16 == 0, "must be align");
	cbd.ByteWidth = sizeof(PerFrame);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	cbd.MiscFlags = 0;

	HR_CHECK(device->Get()->CreateBuffer(&cbd, NULL, m_FrameCB->GetAddress()));
	Vertex p;
	ZeroMemory(&p, sizeof(Vertex));
	p.Age = 0.0f;
	p.Type = 0;

	m_SamLinear = m_ResourceManager->Get<Sampler>(L"Linear").lock();

	// ��Ʈ�� ��¿� ���� ���� �����
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Vertex) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	//std::make_shared<VertexBuffer>(device, vbd, static_cast<D3D11_SUBRESOURCE_DATA>(nullptr), sizeof(Vertex));
	m_InitVB = m_ResourceManager->Create<VertexBuffer>(L"ParticleInit_VB", vbd, vinitData, sizeof(Vertex)).lock();

	vbd.ByteWidth = sizeof(Vertex) * m_MaxParticles; // �ִ�� �ٷ� ���� ũ��. �Ѿ�� �ȵȴ�.
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; // �÷��� �߿�!
	//HR(device->CreateBuffer(&vbd, 0, &mDrawVB));
	m_DrawVB =  m_ResourceManager->Create<VertexBuffer>(L"ParticleDraw_VB", vbd, static_cast<D3D11_SUBRESOURCE_DATA>(nullptr), sizeof(Vertex)).lock();
	//HR(device->CreateBuffer(&vbd, 0, &mStreamOutVB));
	m_StreamOutVB = m_ResourceManager->Create<VertexBuffer>(L"ParticleStreamOut_VB", vbd, static_cast<D3D11_SUBRESOURCE_DATA>(nullptr), sizeof(Vertex)).lock();

	std::vector<std::wstring> flares;
	flares.push_back(L"flare0.dds");
	D3DUtill::CreateTexture2DArraySRV(device->Get(), device->Context(), m_TexArraySRV.GetAddressOf(), flares);
	D3DUtill::CreateRandomTexture1DSRV(device->Get(), m_RandomTexSRV.GetAddressOf());
}

void ParticleObject::Update(float deltaTime, float totalGameTime)
{
	m_DeltaTime = deltaTime;
	m_TotalGameTime = totalGameTime;

	m_Age += deltaTime;
}

void ParticleObject::Draw(const std::shared_ptr<Device>& device, const DirectX::SimpleMath::Matrix& view,
	const DirectX::SimpleMath::Matrix& proj)
{
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	DirectX::SimpleMath::Matrix VP = view * proj;

	// ����
	m_PerFrame.ViewProj = VP.Transpose();
	m_PerFrame.GameTime = m_TotalGameTime;
	m_PerFrame.TimeStep = m_TimeStep;
	m_PerFrame.EyePosW = m_EyePosW;
	m_PerFrame.EmitPosW = m_EmitPosW;
	m_PerFrame.EmitDirW = m_EmitDirW;

	m_FrameCB->Update(m_PerFrame);

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = device->Context();

	// ���ε�
	context->IASetInputLayout(m_DrawVS->InputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	if (m_FirstRun)	// ó�� �׸� �� �ʱ�ȭ ���۷� �׸���.
		context->IASetVertexBuffers(0, 1, m_InitVB->GetAddress(), &stride, &offset);
	else
		context->IASetVertexBuffers(0, 1, m_DrawVB->GetAddress(), &stride, &offset);

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

	// ��Ʈ�� �ƿ����� ���� ���� �� ����
	context->VSSetShader(m_StreamOutVS->GetVS(), 0, 0);
	context->GSSetShader(m_StreamOutGS->GetShader(), 0, 0);
	context->PSSetShader(nullptr, nullptr, 0);

	context->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"DisableDepth").lock()->GetState().Get(), 0);
	context->SOSetTargets(1,	// ���� ���� ����, �ִ� 4��
		m_StreamOutVB->GetAddress(),	// ��¿� ����
		&offset);	// ���� ���۸��� ����ϱ� ������ ��ġ�� ��Ÿ����. ���� 0

	if (m_FirstRun)
	{
		context->Draw(1, 0);
		m_FirstRun = false;
	}
	else
	{
		context->DrawAuto();	// ���������� GPU�� �˾Ƽ� ���� ������ŭ draw �� ���ش�.
	}

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
	context->IASetVertexBuffers(0, 1, m_DrawVB->GetAddress(), &stride, &offset);
	context->DrawAuto();

	// ����ε�
	context->OMSetBlendState(nullptr, factor, 0xffffffff);
	context->OMSetDepthStencilState(nullptr, 0);

	// �⺻ ������ �缳��
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
