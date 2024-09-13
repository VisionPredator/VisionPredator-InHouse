#include "pch.h"
#include "OutlineBlurPass.h"

#include <memory>

#include "Device.h"
#include "ResourceManager.h"
#include "ShaderResourceView.h"
#include "Sampler.h"
#include "ViewPort.h"


void OutlineBlurPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	const uint32_t width = m_Device->GetWndWidth();
	const uint32_t height = m_Device->GetWndHeight();

	m_DownScaledRTV = m_ResourceManager->Create<RenderTargetView>(L"DownScaledRTV").lock();
	m_DownScaledRTV->CreateDownscaledView(2);

	m_OutlineBlurRTV = m_ResourceManager->Create<RenderTargetView>(L"OutlineBlurRTV", RenderTargetViewType::Outline, width, height).lock();

	auto outlineEdgeDetectRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineEdgeDetectRTV").lock();
	m_OutlineEdgeDetectSRV = std::make_shared<ShaderResourceView>(m_Device, outlineEdgeDetectRTV);

	m_DefaultSS = m_ResourceManager->Get<Sampler>(L"Default").lock();
	m_OutlineBlurVS = std::make_shared<VertexShader>(device, L"OutlineBlurVS", "main");
	m_OutlineBlurPS = std::make_shared<PixelShader>(device, L"OutlineBlurPS", "main");
	m_OutlineAddPS = std::make_shared<PixelShader>(device, L"OutlineBlurPS", "add");

	m_QuadVB = m_ResourceManager->Get<VertexBuffer>(L"Quad_VB").lock();
	m_QuadIB = m_ResourceManager->Get<IndexBuffer>(L"Quad_IB").lock();
}

void OutlineBlurPass::Render()
{
	// BLUR PASS 1
	// 아웃라인에 블러 효과를 주어 픽셀 계단현상을 줄인다.
	FLOAT black[4]{ 0.f, 0.f, 0.f, 1.f };
	m_Device->Context()->ClearRenderTargetView(m_DownScaledRTV->Get(), black);

	const uint32_t width = m_Device->GetWndWidth();
	const uint32_t height = m_Device->GetWndHeight();

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(width) / 2.0f;  // 축소된 폭
	viewport.Height = static_cast<float>(height) / 2.0f;  // 축소된 높이
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	m_Device->Context()->RSSetViewports(1, &viewport);

	m_Device->Context()->OMSetRenderTargets(1, m_DownScaledRTV->GetAddress(), nullptr);

	m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Device->Context()->IASetInputLayout(m_OutlineBlurVS->InputLayout());
	m_Device->Context()->IASetVertexBuffers(0, 1, m_QuadVB->GetAddress(), m_QuadVB->Size(), m_QuadVB->Offset());
	m_Device->Context()->IASetIndexBuffer(m_QuadIB->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device->Context()->VSSetShader(m_OutlineBlurVS->GetShader(), nullptr, 0);

	m_Device->Context()->PSSetShader(m_OutlineBlurPS->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetSamplers(0, 1, m_DefaultSS->GetAddress());
	m_Device->Context()->PSSetShaderResources(0, 1, m_OutlineEdgeDetectSRV->GetAddress());

	m_Device->Context()->DrawIndexed(6, 0, 0);

	// 뷰포트 원상복귀
	viewport.Width = static_cast<float>(width);
	viewport.Height = static_cast<float>(height);
	m_Device->Context()->RSSetViewports(1, &viewport);

	// BLUR PASS 2
	// 블러 처리한 아웃라인을 게임 화면과 합성
	m_Device->Context()->OMSetRenderTargets(1, m_OutlineBlurRTV->GetAddress(), nullptr);
	m_Device->Context()->PSSetShader(m_OutlineAddPS->GetShader(), nullptr, 0);
	auto srv = std::make_shared<ShaderResourceView>(m_Device, m_DownScaledRTV);
	auto srv2 = std::make_shared<ShaderResourceView>(m_Device, m_ResourceManager->Get<RenderTargetView>(L"GBuffer").lock());
	m_Device->Context()->PSSetShaderResources(0, 1, srv->GetAddress());
	m_Device->Context()->PSSetShaderResources(1, 1, srv2->GetAddress());
	m_Device->Context()->DrawIndexed(6, 0, 0);
}

void OutlineBlurPass::OnResize()
{
	m_DownScaledRTV = m_ResourceManager->Create<RenderTargetView>(L"DownScaledRTV").lock();
	m_DownScaledRTV->CreateDownscaledView(2);
	m_OutlineBlurRTV->OnResize();

	auto outlineEdgeDetectRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineEdgeDetectRTV").lock();
	m_OutlineEdgeDetectSRV = std::make_shared<ShaderResourceView>(m_Device, outlineEdgeDetectRTV);
}
