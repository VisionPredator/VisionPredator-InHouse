#include "pch.h"
#include "DeferredGeometryPass.h"

#include "Desc.h"


void DeferredGeometryPass::Initialize(Device* device, ResourceManager* resourceManager, uint32_t width, uint32_t height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	m_Viewport.Width = static_cast<FLOAT>(width);
	m_Viewport.Height = static_cast<FLOAT>(height);

	D3D11_TEXTURE2D_DESC depthStencilViewDesc = TextureDESC::DSVDesc;
	depthStencilViewDesc.Width = width;
	depthStencilViewDesc.Height = height;
	m_ResourceManager->Create<DepthStencilView>(L"DeferredGeometryPassDSV", depthStencilViewDesc);
	m_DepthStencilView = m_ResourceManager->Get<DepthStencilView>(L"DeferredGeometryPassDSV");
	// 임시로 여기서 일단 생성하는걸로. 다른 패스에서도 공유해서 사용하면 다른 곳에서 생성하자.



}

void DeferredGeometryPass::Render()
{
}
