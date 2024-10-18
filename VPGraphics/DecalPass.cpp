#include "pch.h"
#include "DecalPass.h"
#include "Slot.h"

DecalPass::DecalPass(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DecalManager> decalmanager) : RenderPass(device,resourceManager)
{
	m_DecalManager = decalmanager;

	m_Device = device;
	m_ResourceManager = resourceManager;

	m_DepthStencilView = resourceManager->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = resourceManager->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = resourceManager->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = resourceManager->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = resourceManager->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRoughnessRTV = resourceManager->Get<RenderTargetView>(L"Metalic_Roughness").lock();
	m_AORTV = resourceManager->Get<RenderTargetView>(L"AO").lock();
	m_EmissiveRTV = resourceManager->Get<RenderTargetView>(L"Emissive").lock();
	m_LightMapRTV = resourceManager->Get<RenderTargetView>(L"LightMap").lock();

	m_QuadVB = resourceManager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = resourceManager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = resourceManager->Get<VertexShader>(L"Quad");
	m_QuadPS = resourceManager->Get<PixelShader>(L"Quad");


	m_AlbedoSRV = resourceManager->Get<ShaderResourceView>(L"Albedo").lock();
	m_NormalSRV = resourceManager->Get<ShaderResourceView>(L"Normal").lock();
	m_PositionSRV = resourceManager->Get<ShaderResourceView>(L"Position").lock();
	m_DepthSRV = resourceManager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughnessSRV = resourceManager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AmbientOcclusionSRV = resourceManager->Get<ShaderResourceView>(L"AO").lock();
	m_EmissiveSRV = resourceManager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBufferSRV = resourceManager->Get<ShaderResourceView>(L"GBuffer").lock();


	//데칼 최대 1천개까지 가능
	for (int i = 0; i < 1000; i++)
	{
		InstanceDecalData temp;
		m_InstanceDatas.push_back(temp);
	}
	m_InstanceBuffer = m_ResourceManager.lock()->Create<VertexBuffer>(L"InstanceBuffer", m_InstanceDatas, true);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_Device.lock()->Context()->Map(m_InstanceBuffer.lock()->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	InstanceDecalData* dataView = reinterpret_cast<InstanceDecalData*>(mappedData.pData);
	dataView = nullptr;
	m_Device.lock()->Context()->Unmap(m_InstanceBuffer.lock()->Get(), 0);


}

DecalPass::~DecalPass()
{

}

void DecalPass::Render()
{

	//instance buffer update
	if (!m_InstanceDatas.empty())
	{

		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_Device.lock()->Context()->Map(m_InstanceBuffer.lock()->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		InstanceDecalData* dataView = reinterpret_cast<InstanceDecalData*>(mappedData.pData);

		for (int i = 0; i < m_InstanceDatas.size(); i++)
		{
			dataView[i] = m_InstanceDatas[i];
		}
		m_Device.lock()->Context()->Unmap(m_InstanceBuffer.lock()->Get(), 0);
	}


	//bind
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
	std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	//set vb(instance buffer)
	// UINT strides[2];
	//strides[0] = sizeof(BaseVertex);
	//strides[1] = sizeof(InstanceDecalData);
	//UINT offsets[2];
	//offsets[0] = 0;
	//offsets[1] = 0;
	//m_Device.lock()->Context()->IASetVertexBuffers();
	
	//set inputlayout,ib
	//m_Device.lock()->Context()->IASetInputLayout();
	//m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	//set primitive
	//m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//set vs, ps
	//m_Device.lock()->Context()->VSSetShader();
	//m_Device.lock()->Context()->PSSetShader();

	//set sampler
	//m_Device.lock()->Context()->PSSetSamplers();

	//set cb
	//Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
	//Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());

	//set srv
	//m_Device.lock()->Context()->PSSetShaderResources();

	//set rtv,dsv
	//Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView.lock()->Get());


	

	//render
	std::map<std::string, std::vector<decal::Info>>& curDecals = m_DecalManager->GetDecals();
	//m_Device.lock()->Context()->DrawIndexedInstanced(mesh->IBCount(), 인스턴스 수, 0, 0, instance buffer 시작 지점offset);




	m_DecalManager->ClearDecals();
}

void DecalPass::OnResize()
{
	std::shared_ptr<ResourceManager> manager = m_ResourceManager.lock();

	m_DepthStencilView = manager->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = manager->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = manager->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = manager->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = manager->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRoughnessRTV = manager->Get<RenderTargetView>(L"Metalic_Roughness").lock();
	m_AORTV = manager->Get<RenderTargetView>(L"AO").lock();
	m_EmissiveRTV = manager->Get<RenderTargetView>(L"Emissive").lock();
	m_LightMapRTV = manager->Get<RenderTargetView>(L"LightMap").lock();

	m_AlbedoSRV = manager->Get<ShaderResourceView>(L"Albedo").lock();
	m_NormalSRV = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_PositionSRV = manager->Get<ShaderResourceView>(L"Position").lock();
	m_DepthSRV = manager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughnessSRV = manager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AmbientOcclusionSRV = manager->Get<ShaderResourceView>(L"AO").lock();
	m_EmissiveSRV = manager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBufferSRV = manager->Get<ShaderResourceView>(L"GBuffer").lock();
	m_LightMapSRV = manager->Get<ShaderResourceView>(L"LightMap").lock();
}
