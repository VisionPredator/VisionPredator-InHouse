#include "pch.h"
#include "FadeInFadeOut.h"
#include "ResourceManager.h"
#include "StaticData.h"

FadeInFadeOut::FadeInFadeOut(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass()
{
	m_ResourceManager = manager;
	m_Device = device;

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_FadeInFadeOut= manager->Get<PixelShader>(L"FadeInFadeOut");
	m_Gbuffer= manager->Get<ShaderResourceView>(L"GBuffer").lock();

}

FadeInFadeOut::~FadeInFadeOut()
{

}

void FadeInFadeOut::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<VertexBuffer> vb = m_QuadVB.lock();
	std::shared_ptr<IndexBuffer> ib = m_QuadIB.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	Device->UnBindSRV();

	Device->BindVS(m_QuadVS.lock());
	Device->Context()->PSSetShader(m_FadeInFadeOut.lock()->GetPS(), nullptr, 0);


	Device->Context()->PSSetShaderResources(0, 1, m_Gbuffer.lock()->GetAddress());

	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);


	std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();
	std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Deferred").lock();


	Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
	Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
	Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);

}

void FadeInFadeOut::OnResize()
{
	std::shared_ptr<ResourceManager> manager = m_ResourceManager.lock();
	std::shared_ptr<Sampler> linear = manager->Get<Sampler>(L"LinearClamp").lock();

	m_Gbuffer= manager->Get<ShaderResourceView>(L"GBuffer").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_FadeInFadeOut = manager->Get<PixelShader>(L"FadeInFadeOut");
}