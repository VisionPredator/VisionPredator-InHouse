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

	m_OutlineEdgeDetectVS = m_ResourceManager->Create<VertexShader>(L"OutlineEdgeDetectVS", L"OutlineEdgeDetectVS", "main").lock();
	m_OutlineEdgeDetectPS = m_ResourceManager->Create<PixelShader>(L"OutlineEdgeDetectPS", L"OutlineEdgeDetectPS", "main").lock();

	m_ObjectMaskSRV = m_ResourceManager->Get<ShaderResourceView>(L"ObjectMaskSRV").lock();

	m_QuadVB = m_ResourceManager->Get<VertexBuffer>(L"Quad_VB").lock();
	m_QuadIB = m_ResourceManager->Get<IndexBuffer>(L"Quad_IB").lock();

	//m_ScreenSizeCB = std::make_shared<ConstantBuffer<ScreenSizeCB>>()
}

void OutlineEdgeDetectPass::Render()
{

}

void OutlineEdgeDetectPass::OnResize()
{

}
