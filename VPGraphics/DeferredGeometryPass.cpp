#include "pch.h"
#include "DeferredGeometryPass.h"

#include "Desc.h"


//void DeferredGeometryPass::Initialize(Device* device, ResourceManager* resourceManager, const uint32_t& width, const uint32_t& height)
//{
//	m_Device = device;
//	m_ResourceManager = resourceManager;
//
//	m_Viewport.Width = static_cast<FLOAT>(width);
//	m_Viewport.Height = static_cast<FLOAT>(height);
//
//	D3D11_TEXTURE2D_DESC depthStencilViewDesc = TextureDESC::DSVDesc;
//	depthStencilViewDesc.Width = width;
//	depthStencilViewDesc.Height = height;
//	m_ResourceManager->Create<DepthStencilView>(L"DeferredGeometryPassDSV", depthStencilViewDesc);
//	m_DepthStencilView = m_ResourceManager->Get<DepthStencilView>(L"DeferredGeometryPassDSV");
//	// 임시로 여기서 일단 생성하는걸로. 다른 패스에서도 공유해서 사용하면 다른 곳에서 생성하자.
//
//	// MRT 갯수 만큼 RTV 생성
//	m_ResourceManager->Create<RenderTargetView>(L"MRT_1", RenderTargetViewType::OffScreen, width, height);
//	m_ResourceManager->Create<RenderTargetView>(L"MRT_2", RenderTargetViewType::OffScreen, width, height);
//	m_ResourceManager->Create<RenderTargetView>(L"MRT_3", RenderTargetViewType::OffScreen, width, height);
//	m_ResourceManager->Create<RenderTargetView>(L"MRT_4", RenderTargetViewType::OffScreen, width, height);
//	m_AlbedoRTV = m_ResourceManager->Get<RenderTargetView>(L"MRT_1");
//	m_NormalRTV = m_ResourceManager->Get<RenderTargetView>(L"MRT_2");
//	m_PositionRTV = m_ResourceManager->Get<RenderTargetView>(L"MRT_3");
//	m_DepthRTV = m_ResourceManager->Get<RenderTargetView>(L"MRT_4");
//
//	// MRT의 텍스처를 참조하는 SRV도 생성
//	m_ResourceManager->Create<ShaderResourceView>(L"DeferredSRV_1", m_AlbedoRTV);
//	m_ResourceManager->Create<ShaderResourceView>(L"DeferredSRV_2", m_NormalRTV);
//	m_ResourceManager->Create<ShaderResourceView>(L"DeferredSRV_3", m_PositionRTV);
//	m_ResourceManager->Create<ShaderResourceView>(L"DeferredSRV_4", m_DepthRTV);
//
//	
//	//const std::wstring psPath = L"LightPass_";
//	//const std::wstring vsPath = L"LightPass__";
//
//	//m_ResourceManager->Create<PixelShader>(psPath, L"")
//}

void DeferredGeometryPass::Initialize(std::shared_ptr<Device>& device,
	std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	m_Viewport.Width = static_cast<FLOAT>(width);
	m_Viewport.Height = static_cast<FLOAT>(height);

	D3D11_TEXTURE2D_DESC depthStencilViewDesc = TextureDESC::DSVDesc;
	depthStencilViewDesc.Width = width;
	depthStencilViewDesc.Height = height;
	
}

void DeferredGeometryPass::Render()
{

}
