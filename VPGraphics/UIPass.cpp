#include "pch.h"
#include "UIPass.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"
#include "UIManager.h"

#include "DepthStencilState.h"
#include "BlendState.h"

void UIPass::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager,
	const std::shared_ptr<UIManager>& uiManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_UIManager = uiManager;

#pragma region TEST
	//ui::ImageInfo testInfo = {};
	//testInfo.Layer = 1;
	//m_UIManager->CreateImageObject(518, testInfo);

	//ui::ImageInfo testInfo2 = {};
	//testInfo2.ImagePath = "GOSEGUI0900.png";
	//testInfo2.Layer = 2;
	//testInfo2.Height = 200;
	//testInfo2.Width = 200;
	//m_UIManager->CreateImageObject(519, testInfo2);

	//ui::ImageInfo testInfo3 = {};
	//testInfo3.ImagePath = "GOSEGUI0900.png";
	//testInfo3.StartPosX = 800.f;
	//testInfo3.StartPosY = 300.f;
	//testInfo3.Layer = 1;
	//m_UIManager->CreateImageObject(600, testInfo3);
#pragma endregion

	m_ImageTransformCB = m_ResourceManager->Create<ConstantBuffer<ImageTransformCB>>(L"ImageTransformCB", BufferDESC::Constant::DefaultTransform).lock();

	m_VertexShader = std::make_shared<VertexShader>(m_Device, L"Sprite2DVS", "main");
	m_PixelShader = std::make_shared<PixelShader>(m_Device, L"Sprite2DPS", "main");
}

void UIPass::Render()
{
	std::shared_ptr<Sampler> linear = m_ResourceManager->Get<Sampler>(L"Linear").lock();
	m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 알파 블랜딩 사용.
	float factor[] = { 0.f, 0.f, 0.f, 0.f };
	m_Device->Context()->OMSetBlendState(m_ResourceManager->Get<BlendState>(L"AlphaBlending").lock()->GetState().Get(), factor, 0xffffffff);	// 가산 혼합

	// 2D 렌더링을 위해 Z 버퍼 끄기
	m_Device->Context()->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"DisableDepth").lock()->GetState().Get(), 0);

	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock();

	ImageTransformCB transform;
	transform.World = VPMath::Matrix::Identity.Transpose();
	transform.View = CameraCB->m_struct.view;
	transform.Projection = CameraCB->m_struct.orthoProj;
	m_ImageTransformCB->Update(transform);

	m_Device->Context()->IASetInputLayout(m_VertexShader->InputLayout());
	m_Device->Context()->VSSetConstantBuffers(0, 1, m_ImageTransformCB->GetAddress());
	m_Device->Context()->VSSetShader(m_VertexShader->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetConstantBuffers(0, 1, m_ImageTransformCB->GetAddress());
	m_Device->Context()->PSSetShader(m_PixelShader->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());


	// Execute Draw Call for all 2D Objects
	m_UIManager->Render();

	// TODO: 2D 렌더링이 완료되었으므로 다시 Z 버퍼 키기
	m_Device->Context()->OMSetDepthStencilState(m_ResourceManager->Get<DepthStencilState>(L"DefaultDSS").lock()->GetState().Get(), 0);
}
