#include "pch.h"
#include "ObjectMaskPass.h"
#include "Device.h"
#include "ResourceManager.h"

ObjectMaskPass::ObjectMaskPass(std::shared_ptr<Device> device,
	std::shared_ptr<ResourceManager> resourceManager)
		: RenderPass(device, resourceManager)
{
	const uint32_t width = m_Device.lock()->GetWndWidth();
	const uint32_t height = m_Device.lock()->GetWndHeight();

	m_ObjectMaskRTV = m_ResourceManager.lock()->Create<RenderTargetView>(L"ObjectMaskRTV", RenderTargetViewType::ObjectMask, width, height).lock();
	m_DefaultDSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"Default").lock();
	m_ObjectMaskPS = m_ResourceManager.lock()->Create<PixelShader>(L"ObjectMask", L"ObjectMask").lock();
	m_ObjectMaskStaticMeshVS = m_ResourceManager.lock()->Create<VertexShader>(L"ObjectMaskVS", L"ObjectMaskVS", "main").lock();
}

void ObjectMaskPass::OnResize()
{

}

void ObjectMaskPass::Render()
{

}
