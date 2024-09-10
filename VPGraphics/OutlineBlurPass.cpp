#include "pch.h"
#include "OutlineBlurPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "ShaderResourceView.h"
#include "Sampler.h"


void OutlineBlurPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	const uint32_t width = m_Device->GetWndWidth();
	const uint32_t height = m_Device->GetWndHeight();

	m_OutlineBlurRTV = m_ResourceManager->Create<RenderTargetView>(L"OutlineBlurRTV", RenderTargetViewType::Outline, width, height).lock();
	auto outlineEdgeDetectRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineEdgeDetectRTV").lock();
	m_OutlineEdgeDetectSRV = std::make_shared<ShaderResourceView>(m_Device, outlineEdgeDetectRTV);

	m_DefaultSS = m_ResourceManager->Get<Sampler>(L"Default").lock();
	m_OutlineBlurVS = std::make_shared<VertexShader>(device, L"OutlineBlurVS", "main");
	m_OutlineBlurPS = std::make_shared<PixelShader>(device, L"OutlineBlurPS", "main");

	m_QuadVB = m_ResourceManager->Get<VertexBuffer>(L"Quad_VB").lock();
	m_QuadIB = m_ResourceManager->Get<IndexBuffer>(L"Quad_IB").lock();
}

void OutlineBlurPass::Render()
{


	FLOAT black[4]{ 0.f, 0.f, 0.f, 1.f };

	auto gbufferrtv = m_ResourceManager->Get<RenderTargetView>(L"GBuffer").lock();
	auto gbuffersrv = std::make_shared<ShaderResourceView>(m_Device, gbufferrtv);

	m_Device->Context()->ClearRenderTargetView(m_OutlineBlurRTV->Get(), black);
	m_Device->Context()->OMSetRenderTargets(1, m_OutlineBlurRTV->GetAddress(), nullptr);

	m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Device->Context()->IASetInputLayout(m_OutlineBlurVS->InputLayout());
	m_Device->Context()->IASetVertexBuffers(0, 1, m_QuadVB->GetAddress(), m_QuadVB->Size(), m_QuadVB->Offset());
	m_Device->Context()->IASetIndexBuffer(m_QuadIB->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device->Context()->VSSetShader(m_OutlineBlurVS->GetShader(), nullptr, 0);

	m_Device->Context()->PSSetShader(m_OutlineBlurPS->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetSamplers(0, 1, m_DefaultSS->GetAddress());
	m_Device->Context()->PSSetShaderResources(0, 1, m_OutlineEdgeDetectSRV->GetAddress());
	m_Device->Context()->PSSetShaderResources(1, 1, gbuffersrv->GetAddress());

	m_Device->Context()->DrawIndexed(6, 0, 0);
}

void OutlineBlurPass::OnResize()
{
	m_OutlineBlurRTV->OnResize();
	auto outlineEdgeDetectRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineEdgeDetectRTV").lock();
	m_OutlineEdgeDetectSRV = std::make_shared<ShaderResourceView>(m_Device, outlineEdgeDetectRTV);
}
