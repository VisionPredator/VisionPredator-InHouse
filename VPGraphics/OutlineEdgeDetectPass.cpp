#include "pch.h"
#include "OutlineEdgeDetectPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "Sampler.h"
#include "VertexBuffer.h"
#include "RenderTargetView.h"

void OutlineEdgeDetectPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	const uint32_t width = m_Device->GetWndWidth();
	const uint32_t height = m_Device->GetWndHeight();

	m_OutlineEdgeDetectRTV = m_ResourceManager->Create<RenderTargetView>(L"OutlineEdgeDetectRTV", RenderTargetViewType::Outline, width, height).lock();

	auto ObjectMaskRTV = m_ResourceManager->Get<RenderTargetView>(L"ObjectMaskRTV").lock();
	m_ObjectMaskSRV = std::make_shared<ShaderResourceView>(device, ObjectMaskRTV);

	m_LinearClampSS = m_ResourceManager->Get<Sampler>(L"LinearClamp").lock();
	m_OutlineEdgeDetectVS = m_ResourceManager->Create<VertexShader>(L"OutlineEdgeDetectVS", L"OutlineEdgeDetectVS", "main").lock();
	m_OutlineEdgeDetectPS = m_ResourceManager->Create<PixelShader>(L"OutlineEdgeDetectPS", L"OutlineEdgeDetectPS", "main").lock();

	m_QuadVB = m_ResourceManager->Get<VertexBuffer>(L"Quad_VB").lock();
	m_QuadIB = m_ResourceManager->Get<IndexBuffer>(L"Quad_IB").lock();
	m_ScreenSizeCB = std::make_shared<ConstantBuffer<ScreenSizeCB>>(device, ConstantBufferType::Default);
}

void OutlineEdgeDetectPass::Render()
{
	m_ScreenSize.Width = m_Device->GetWndWidth();
	m_ScreenSize.Height = m_Device->GetWndHeight();
	m_ScreenSizeCB->Update(m_ScreenSize);

	FLOAT black[4]{ 0.f, 0.f, 0.f, 1.f };
	m_Device->Context()->ClearRenderTargetView(m_OutlineEdgeDetectRTV->Get(), black);
	m_Device->Context()->OMSetRenderTargets(1, m_OutlineEdgeDetectRTV->GetAddress(), nullptr);

	m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Device->Context()->IASetInputLayout(m_OutlineEdgeDetectVS->InputLayout());
	m_Device->Context()->IASetVertexBuffers(0, 1, m_QuadVB->GetAddress(), m_QuadVB->Size(), m_QuadVB->Offset());
	m_Device->Context()->IASetIndexBuffer(m_QuadIB->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device->Context()->VSSetShader(m_OutlineEdgeDetectVS->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetShader(m_OutlineEdgeDetectPS->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetSamplers(0, 1, m_LinearClampSS->GetAddress());
	m_Device->Context()->PSSetConstantBuffers(0, 1, m_ScreenSizeCB->GetAddress());
	m_Device->Context()->PSSetShaderResources(0, 1, m_ObjectMaskSRV->GetAddress());

	m_Device->Context()->DrawIndexed(6, 0, 0);
}

void OutlineEdgeDetectPass::OnResize()
{
	// TODO OutlineEdgeDetectPass Reszie
	// SRV Àç»ý¼º
}
