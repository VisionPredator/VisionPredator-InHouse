#include "pch.h"
#include "DeferredLightPass.h"

#include "Device.h"
#include "ResourceManager.h"

#include"StaticData.h"
#include "Slot.h"



DeferredLightPass::DeferredLightPass(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{
	m_DepthStencilView = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_Albedo = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Albedo").lock();
	m_Normal = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Normal").lock();
	m_Position = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Position").lock();
	m_Depth = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Depth").lock();
	m_Metalic = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Metalic").lock();
	m_Roughness = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Roughness").lock();
	m_AO = m_ResourceManager.lock()->Get<ShaderResourceView>(L"AO").lock();
	m_GBuffer = m_ResourceManager.lock()->Get<ShaderResourceView>(L"GBuffer").lock();


	m_PS = resourceManager->Get<PixelShader>(L"MeshDeferredLight");
	m_VS = resourceManager->Get<VertexShader>(L"Quad");

	m_QuadVB = resourceManager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = resourceManager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadPS = resourceManager->Get<PixelShader>(L"Quad");


}



DeferredLightPass::~DeferredLightPass()
{

}

void DeferredLightPass::Render()
{

	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();
	std::shared_ptr<VertexBuffer> vb = m_QuadVB.lock();
	std::shared_ptr<IndexBuffer> ib = m_QuadIB.lock();

	//Save GBuffer texture
	{
		std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();
		std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Deferred").lock();

		Device->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
		Device->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
		Device->Context()->PSSetShader(m_PS.lock()->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, m_Albedo->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, m_Normal->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Position), 1, m_Position->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Depth), 1, m_Depth->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, m_Metalic->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Roughness), 1, m_Roughness->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, m_AO->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), dsv->Get());
		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}


	//draw quad
	{
		std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"RTV_Main").lock();
		std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Main").lock();

		Device->UnBindSRV();

		Device->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
		Device->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
		Device->Context()->PSSetShader(m_QuadPS.lock()->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::GBuffer), 1, m_GBuffer->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}


}

