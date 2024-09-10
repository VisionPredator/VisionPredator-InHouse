#include "pch.h"
#include "OutlineAddPass.h"
#include "Device.h"
#include "ResourceManager.h"

void OutlineAddPass::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	const uint32_t width = m_Device->GetWndWidth();
	const uint32_t height = m_Device->GetWndHeight();

	m_OffScreenRTV = m_ResourceManager->Get<RenderTargetView>(L"GBuffer").lock();
	auto outlineBlurRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineBlurRTV").lock();
	m_OutlineBlurSRV = std::make_shared<ShaderResourceView>(m_Device, outlineBlurRTV);

	m_DefaultDSV = m_ResourceManager->Get<DepthStencilView>(L"DSV_Main").lock();

	m_DefaultSS = m_ResourceManager->Get<Sampler>(L"Default").lock();
	m_OutlineAddVS = std::make_shared<VertexShader>(device, L"OutlineAddVS", "main");
	m_OutlineAddPS = std::make_shared<PixelShader>(device, L"OutlineAddPS", "main");

	m_QuadVB = m_ResourceManager->Get<VertexBuffer>(L"Quad_VB").lock();
	m_QuadIB = m_ResourceManager->Get<IndexBuffer>(L"Quad_IB").lock();
}

void OutlineAddPass::Render()
{
	m_Device->Context()->OMSetRenderTargets(1, m_OffScreenRTV->GetAddress(), m_DefaultDSV->Get());

	m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Device->Context()->IASetInputLayout(m_OutlineAddVS->InputLayout());
	m_Device->Context()->IASetVertexBuffers(0, 1, m_QuadVB->GetAddress(), m_QuadVB->Size(), m_QuadVB->Offset());
	m_Device->Context()->IASetIndexBuffer(m_QuadIB->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device->Context()->VSSetShader(m_OutlineAddVS->GetShader(), nullptr, 0);

	m_Device->Context()->PSSetShader(m_OutlineAddPS->GetShader(), nullptr, 0);
	m_Device->Context()->PSSetSamplers(0, 1, m_DefaultSS->GetAddress());
	m_Device->Context()->PSSetShaderResources(0, 1, m_OutlineBlurSRV->GetAddress());

	m_Device->Context()->DrawIndexed(6, 0, 0);
}

void OutlineAddPass::OnResize()
{
	m_OffScreenRTV = m_ResourceManager->Get<RenderTargetView>(L"GBuffer").lock();
	auto outlineBlurRTV = m_ResourceManager->Get<RenderTargetView>(L"OutlineBlurRTV").lock();
	m_OutlineBlurSRV = std::make_shared<ShaderResourceView>(m_Device, outlineBlurRTV);
	m_DefaultDSV = m_ResourceManager->Get<DepthStencilView>(L"DSV_Main").lock();
}
