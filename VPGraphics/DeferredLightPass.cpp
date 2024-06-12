#include "pch.h"
#include "DeferredLightPass.h"

#include "Device.h"
#include "ResourceManager.h"

#include"StaticData.h"


void DeferredLightPass::Initialize(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager,
	const uint32_t& width, const uint32_t& height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;


	m_Albedo = resourceManager->Get<ShaderResourceView>(L"OffScreenSRV_1");
	m_Normal = resourceManager->Get<ShaderResourceView>(L"OffScreenSRV_2");
	m_Position = resourceManager->Get<ShaderResourceView>(L"OffScreenSRV_3");
	m_Depth = resourceManager->Get<ShaderResourceView>(L"OffScreenSRV_4");
	m_Tangent = resourceManager->Get<ShaderResourceView>(L"OffScreenSRV_5");

	m_PS = resourceManager->Get<PixelShader>(L"MeshDeferredLight");
	m_VS = resourceManager->Get<VertexShader>(L"Quad");
}

void DeferredLightPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<RenderTargetView> rtv = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_1").lock();
	std::shared_ptr<DepthStencilView> dsv = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_1").lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	
//bind
	Device->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	Device->Context()->VSSetShader(m_VS.lock()->GetVS(),nullptr,0);
	Device->Context()->PSSetShader(m_PS.lock()->GetPS(),nullptr,0);
	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
	m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	Device->Context()->PSSetShaderResources(0,1,m_Albedo.lock()->GetAddress());
	Device->Context()->PSSetShaderResources(1,1,m_Normal.lock()->GetAddress());
	Device->Context()->PSSetShaderResources(2,1,m_Position.lock()->GetAddress());
	Device->Context()->PSSetShaderResources(3,1,m_Depth.lock()->GetAddress());
	Device->Context()->PSSetShaderResources(4,1,m_Tangent.lock()->GetAddress());

	Device->Context()->PSSetSamplers(0, 1, m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock()->GetAddress());

	Device->Context()->OMSetRenderTargets(1,rtv->GetAddress(),dsv->Get());

	Device->Context()->DrawIndexed(Quad::Index::count,0,0);
}
