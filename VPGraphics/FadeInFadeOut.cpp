#include "pch.h"
#include "FadeInFadeOut.h"
#include "ResourceManager.h"
#include "StaticData.h"
#include "Slot.h"

FadeInFadeOut::FadeInFadeOut(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass()
{
	m_ResourceManager = manager;
	m_Device = device;
	UINT width = m_Device.lock()->GetWndSize().right - m_Device.lock()->GetWndSize().left;
	UINT height = m_Device.lock()->GetWndSize().bottom - m_Device.lock()->GetWndSize().top;

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_QuadPS = manager->Get<PixelShader>(L"Quad");
	m_FadeInFadeOut= manager->Get<PixelShader>(L"FadeInFadeOut");
	m_Gbuffer= manager->Get<ShaderResourceView>(L"GBuffer").lock();

	m_FadeInOutRTV = manager->Create<RenderTargetView>(L"FadeInOutRTV", RenderTargetViewType::OffScreen,width,height).lock();
	m_FadeInOutSRV = manager->Create<ShaderResourceView>(L"FadeInOutSRV",m_FadeInOutRTV.lock()).lock();
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

	//srv 만들기
	Device->UnBindSRV();

	Device->BindVS(m_QuadVS.lock());
	Device->Context()->PSSetShader(m_FadeInFadeOut.lock()->GetPS(), nullptr, 0);
	Device->Context()->PSSetShaderResources(0, 1, m_Gbuffer.lock()->GetAddress());

	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	//std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();

	Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
	Device->Context()->OMSetRenderTargets(1, m_FadeInOutRTV.lock()->GetAddress(), nullptr);
	Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);



	//gbuffer에 다시 그리기
	Device->UnBindSRV();

	Device->BindVS(m_QuadVS.lock());
	Device->Context()->PSSetShader(m_QuadPS.lock()->GetPS(), nullptr, 0);
	Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::GBuffer), 1, m_FadeInOutSRV.lock()->GetAddress());

	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();

	Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
	Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
	Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);




}

void FadeInFadeOut::OnResize()
{
	std::shared_ptr<ResourceManager> manager = m_ResourceManager.lock();

	m_Gbuffer= manager->Get<ShaderResourceView>(L"GBuffer").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_FadeInFadeOut = manager->Get<PixelShader>(L"FadeInFadeOut");


	m_FadeInOutRTV.lock()->OnResize();
	manager->Erase<ShaderResourceView>(L"m_FadeInOutSRV");
	m_FadeInOutSRV = manager->Create<ShaderResourceView>(L"m_FadeInOutSRV", m_FadeInOutRTV.lock());
}