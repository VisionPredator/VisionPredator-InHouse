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
	constexpr uint32_t MaxParticles = 1000.f;

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
		
		HR_CHECK(device->Get()->CreateBuffer(&vbd, &vinitData, &m_InitVB));			// 스트림 출력용 버퍼

		vbd.ByteWidth = sizeof(ParticleVertex) * MaxParticles; // 최대로 다룰 버퍼 크기. 넘어가면 안된다.
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; // 플래그 중요!
		HR_CHECK(device->Get()->CreateBuffer(&vbd, nullptr, &m_DrawVB));			// 
		HR_CHECK(device->Get()->CreateBuffer(&vbd, nullptr, &m_StreamOutVB));

		m_FrameCB = m_ResourceManager->Create<ConstantBuffer<PerFrame>>(L"FrameCB", ConstantBufferType::Default).lock();
		m_DataCB = m_ResourceManager->Create<ConstantBuffer<DataCB>>(L"DataCB", ConstantBufferType::Default).lock();
	}

	// Create Shaders
	{
		m_StreamOutVS = std::make_shared<VertexShader>(device, L"ParticleVS", "StreamOutVS");
		m_DrawVS = std::make_shared<VertexShader>(device, L"ParticleVS", "DrawVS");
		m_StreamOutGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "StreamOutGS", "gs_5_0");
		m_DrawGS = std::make_shared<GeometryShader>(device, L"ParticleGS", "DrawGS", "gs_5_0");
		m_DrawPS = std::make_shared<PixelShader>(device, L"ParticlePS", "DrawPS");

		m_SamLinear = m_ResourceManager->Get<Sampler>(L"LinearWrap").lock();
	}

	// TODO: SRV 이용하는 걸로 바꿔야 한다.
	// Create Textures
	{
		std::vector<std::wstring> flares;
		if (!m_Info.TexturePath.empty())
		{
			flares.push_back(Util::ToWideChar(m_Info.TexturePath));
		}
		else
		{
			flares.push_back(Util::ToWideChar("../../../Resource/Texture/Smoke.png"));	
		}
		D3DUtill::CreateTexture2DArraySRV(device->Get(), device->Context(), m_TexArraySRV.GetAddressOf(), flares);

		if (m_Info.TexturePath.empty())
			m_Info.TexturePath = "Smoke.png";
		m_TextureSRV = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.TexturePath).c_str(), Util::ToWideChar(m_Info.TexturePath).c_str()).lock();
		
		// 랜덤 텍스처 생성
		m_RandomTextureSRV = m_ResourceManager->Create<ShaderResourceView>(L"RandomTextureSRV").lock();
		D3DUtill::CreateRandomTexture1DSRV(device->Get(), m_RandomTexSRV.GetAddressOf());
		D3DUtill::CreateRandomTexture1DSRV(device->Get(), m_RandomTextureSRV->GetAddress());
	}
}

void ParticleObject::Update()
{
	if (m_Info.TexturePath.empty())
		m_Info.TexturePath = "Smoke.png";	// 기본 파티클 이미지 세팅

	m_TextureSRV = m_ResourceManager->Create<ShaderResourceView>(Util::ToWideChar(m_Info.TexturePath).c_str(), Util::ToWideChar(m_Info.TexturePath).c_str()).lock();
}

void ParticleObject::Draw(float deltaTime, float totalGameTime)
{
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context = m_Device->Context();
	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
	DirectX::XMFLOAT3 cameraPos = DirectX::XMFLOAT3(CameraCB->m_struct.viewInverse.Transpose()._41, CameraCB->m_struct.viewInverse.Transpose()._42, CameraCB->m_struct.viewInverse.Transpose()._43);

	UINT stride = sizeof(ParticleVertex);
	UINT offset = 0;

	VPMath::Matrix viewInvert = CameraCB->m_struct.view.Invert().Transpose();
	VPMath::Matrix VP = CameraCB->m_struct.view.Transpose() * CameraCB->m_struct.proj.Transpose();

	m_TotalGameTime = totalGameTime;
	m_TimeStep = deltaTime;
	m_Age += deltaTime;

	// CB data 갱신
	m_PerFrame.ViewProj = CameraCB->m_struct.worldviewproj;
	m_PerFrame.EyePosW = cameraPos;
	m_PerFrame.GameTime = m_TotalGameTime;
	m_PerFrame.TimeStep = m_TimeStep;
	m_FrameCB->Update(m_PerFrame);

	const float lifetime = m_Info.StartLifetimeA + static_cast<float>(rand()) / (RAND_MAX / (m_Info.StartLifetimeB - m_Info.StartLifetimeA));
	m_Data.StartLifetime = lifetime;
	const float size = m_Info.StartSizeA + static_cast<float>(rand()) / (RAND_MAX / (m_Info.StartSizeB - m_Info.StartSizeA));
	m_Data.StartSize = VPMath::Vector2{size, size};	// TODO: 가로세로 사이즈 조절할수 있도록 열어두면 좋을듯
	m_Data.IsLoop = m_Info.IsLoop;
	m_Data.EmitDirW = m_Info.Direction;
	m_Data.EmitPosW = m_Info.PosW;
	m_Data.Duration = m_Info.Duration;
	m_DataCB->Update(m_Data);


	// 기존에 바인딩된 꼭짓점 버퍼를 해제합니다.
	ID3D11Buffer* nullBuffers[4] = { nullptr, nullptr, nullptr, nullptr };
	UINT strides[4] = { 0, 0, 0, 0 };
	UINT offsets[4] = { 0, 0, 0, 0 };
	context->IASetVertexBuffers(0, 4, nullBuffers, strides, offsets);

	// Bind
	context->IASetInputLayout(m_DrawVS->InputLayout());
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	if (m_FirstRun)	// 처음 그릴 땐 초기화 버퍼로 그린다.
		context->IASetVertexBuffers(0, 1, m_InitVB.GetAddressOf(), &stride, &offset);
	else
		context->IASetVertexBuffers(0, 1, m_DrawVB.GetAddressOf(), &stride, &offset);

	// Bind Shader's resources
	context->VSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->VSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->VSSetConstantBuffers(1, 1, m_DataCB->GetAddress());
	context->VSSetShaderResources(0, 1, m_TextureSRV->GetAddress());
	context->VSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->GSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->GSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->GSSetConstantBuffers(1, 1, m_DataCB->GetAddress());
	context->GSSetShaderResources(0, 1, m_TextureSRV->GetAddress());
	context->GSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->PSSetSamplers(0, 1, m_SamLinear->GetAddress());
	context->PSSetConstantBuffers(0, 1, m_FrameCB->GetAddress());
	context->PSSetConstantBuffers(1, 1, m_DataCB->GetAddress());
	context->PSSetShaderResources(0, 1, m_TextureSRV->GetAddress());
	context->PSSetShaderResources(1, 1, m_RandomTexSRV.GetAddressOf());

	context->VSSetShader(m_StreamOutVS->GetVS(), nullptr, 0);
	context->GSSetShader(m_StreamOutGS->GetShader(), nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);	// PS 비활성화

	// 인덱스 버퍼 세팅 해제
	// 깊이 버퍼 비활성화
	context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	context->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"DisableDepth").lock()->GetState().Get(), 0);

	/// 1. 스트림 아웃으로 정점 생성 및 수명 끝난 파티클 삭제
	context->SOSetTargets(1,			// 묶을 정점 개수, 최대 4개
		m_StreamOutVB.GetAddressOf(),	// 출력용 버퍼
		&offset);						// 정점 버퍼마다 기록하기 시작할 위치를 나타낸다. 현재 0

	context->OMSetRenderTargets(0, nullptr, nullptr);	// 정점을 생성하는 것이기 때문에 실제로 화면에 그릴 필요가 없다.

	if (m_FirstRun)
	{
		context->Draw(1, 0);
		m_FirstRun = false;
	}
	else
	{
		context->DrawAuto();	// 내부적으로 GPU가 알아서 정점 개수만큼 draw 를 해준다.
	}

	/// 2. 생성한 정점들로 실제 Draw 를 진행
	ID3D11Buffer* bufferArray[1] = { nullptr };
	context->OMSetDepthStencilState(nullptr, 0);	// 언바인드

	context->SOSetTargets(1, bufferArray, &offset);	// 출력 단계를 비워준다.
	std::swap(m_DrawVB, m_StreamOutVB);	// 핑퐁을 위해 swap 한다.

	std::shared_ptr<RenderTargetView> rtv = m_ResourceManager->Get<RenderTargetView>(L"GBuffer").lock();
	std::shared_ptr<DepthStencilView> dsv = m_ResourceManager->Get<DepthStencilView>(L"DSV_Main").lock();
	//m_Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), dsv->Get());
	m_Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);

	// 화면 렌더링
	context->VSSetShader(m_DrawVS->GetShader(), nullptr, 0);
	context->GSSetShader(m_DrawGS->GetShader(), nullptr, 0);
	context->PSSetShader(m_DrawPS->GetShader(), nullptr, 0);

	float factor[] = { 0.f, 0.f, 0.f, 0.f };
	context->OMSetBlendState(m_ResourceManager->Get<BlendState>(L"AdditiveBlending").lock()->GetState().Get(), factor, 0xffffffff);	// 가산 혼합
	context->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"NoDepthWrites").lock()->GetState().Get(), 0);	// 깊이 쓰기는 하지 않는다.
	context->IASetVertexBuffers(0, 1, m_DrawVB.GetAddressOf(), &stride, &offset);
	context->DrawAuto();

	// 언바인드
	context->OMSetBlendState(nullptr, factor, 0xffffffff);
	context->OMSetDepthStencilState(nullptr, 0);

	// 기본 값으로 재설정
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

void ParticleObject::SetParticleInfo(const effect::ParticleInfo& info)
{
	m_Info.Direction = info.Direction;
	m_Info.PosW = info.PosW;
	m_Info.Duration = info.Duration;
	m_Info.TexturePath = info.TexturePath;
	m_Info.IsLoop = info.IsLoop;
	m_Info.StartLifetimeA = info.StartLifetimeA;
	m_Info.StartLifetimeB = info.StartLifetimeB;
	m_Info.StartSizeA = info.StartSizeA;
	m_Info.StartSizeB = info.StartSizeB;
	m_Info.Duration = info.Duration;
	//m_Info.
}
