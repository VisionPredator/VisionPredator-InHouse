#include "pch.h"
#include "OutlineEdgeDetectPass.h"
#include "Device.h"
#include "ResourceManager.h"

void OutlineEdgeDetectPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	uint32_t width = m_Device->GetWndWidth();
	uint32_t height = m_Device->GetWndHeight();

	m_OutlineEdgeDetectRTV = m_ResourceManager->Create<RenderTargetView>(L"OutlineEdgeDetectRTV", RenderTargetViewType::OutlineEdgeDetect, width, height).lock();
	// DSV 는 nullptr 값 바인딩



}

void OutlineEdgeDetectPass::Render()
{

}

void OutlineEdgeDetectPass::OnResize()
{

}
