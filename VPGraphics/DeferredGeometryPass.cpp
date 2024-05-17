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
	// �ӽ÷� ���⼭ �ϴ� �����ϴ°ɷ�. �ٸ� �н������� �����ؼ� ����ϸ� �ٸ� ������ ��������.



}

void DeferredGeometryPass::Render()
{
}
