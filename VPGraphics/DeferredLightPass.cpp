#include "pch.h"
#include "DeferredLightPass.h"

#include "Device.h"
#include "ResourceManager.h"

#include"StaticData.h"


DeferredLightPass::DeferredLightPass(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{
	m_DepthStencilView = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main").lock();

	m_Albedo = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Albedo").lock();
	m_Normal = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Normal").lock();
	m_Position = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Position").lock();
	m_Depth = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Depth").lock();


	m_PS = resourceManager->Get<PixelShader>(L"MeshDeferredLight");
	m_VS = resourceManager->Get<VertexShader>(L"Quad");
}

void DeferredLightPass::Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager,
	const uint32_t& width, const uint32_t& height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;


	//m_AlbedoRTV = resourceManager->Get<ShaderResourceView>(L"Albedo");
	//m_NormalRTV = resourceManager->Get<ShaderResourceView>(L"Normal");
	//m_PositionRTV = resourceManager->Get<ShaderResourceView>(L"Position");
	//m_DepthRTV = resourceManager->Get<ShaderResourceView>(L"Depth");

	m_PS = resourceManager->Get<PixelShader>(L"MeshDeferredLight");
	m_VS = resourceManager->Get<VertexShader>(L"Quad");
}

void DeferredLightPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<RenderTargetView> rtv = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_Main").lock();
	std::shared_ptr<DepthStencilView> dsv = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main").lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();

	Device->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	Device->Context()->VSSetShader(m_VS.lock()->GetVS(),nullptr,0);
	Device->Context()->PSSetShader(m_PS.lock()->GetPS(),nullptr,0);
	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	Device->Context()->PSSetShaderResources(0,1,m_Albedo->GetAddress());
	Device->Context()->PSSetShaderResources(1,1,m_Normal->GetAddress());
	Device->Context()->PSSetShaderResources(2,1,m_Position->GetAddress());
	Device->Context()->PSSetShaderResources(3,1,m_Depth->GetAddress());

	Device->Context()->PSSetSamplers(0, 1, m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock()->GetAddress());

	Device->Context()->OMSetRenderTargets(1,rtv->GetAddress(),dsv->Get());

	Device->Context()->DrawIndexed(Quad::Index::count,0,0);
}
