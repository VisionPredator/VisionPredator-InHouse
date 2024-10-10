#include "pch.h"
#include "DeferredInstancing.h"
#include "ResourceManager.h"
#include "ShaderResourceView.h"
#include "RenderTargetView.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"

#include "Slot.h"
#include "StaticData.h"
#include "DebugDrawManager.h"
#include "LightManager.h"
#include "Device.h"

DeferredInstancing::~DeferredInstancing()
{

}

void DeferredInstancing::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<LightManager>& lightManager)
{
	m_ResourceManager = resourceManager;
	m_Device = device;

	for (int i = 0; i < 100; i++)
	{
	InstanceData temp;
	m_InstanceDatas.push_back(temp);
	}
	m_InstanceBuffer = m_ResourceManager.lock()->Create<VertexBuffer>(L"InstanceBuffer", m_InstanceDatas, true);
	m_InstancingVS = m_ResourceManager.lock()->Create<VertexShader>(L"InstancingVS", L"InstancingVS");

	D3D11_MAPPED_SUBRESOURCE mappedData;
	m_Device.lock()->Context()->Map(m_InstanceBuffer.lock()->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	InstanceData* dataView = reinterpret_cast<InstanceData*>(mappedData.pData);
	dataView = nullptr;
	m_Device.lock()->Context()->Unmap(m_InstanceBuffer.lock()->Get(), 0);

	m_Device = device;
	m_ResourceManager = resourceManager;
	m_LightManager = lightManager;

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

	m_GeometryPS = resourceManager->Get<PixelShader>(L"InstancingPS").lock();
	m_Deferred = resourceManager->Get<PixelShader>(L"MeshDeferredLight");
	m_MeshPS = resourceManager->Get<PixelShader>(L"InstancingPS");

	m_AlbedoSRV = resourceManager->Get<ShaderResourceView>(L"Albedo").lock();
	m_NormalSRV = resourceManager->Get<ShaderResourceView>(L"Normal").lock();
	m_PositionSRV = resourceManager->Get<ShaderResourceView>(L"Position").lock();
	m_DepthSRV = resourceManager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughnessSRV = resourceManager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AmbientOcclusionSRV = resourceManager->Get<ShaderResourceView>(L"AO").lock();
	m_EmissiveSRV = resourceManager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBufferSRV = resourceManager->Get<ShaderResourceView>(L"GBuffer").lock();




}

void DeferredInstancing::Render()
{

	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	// Bind Common Resources
	{
		Device->UnBindSRV();
		std::vector<ID3D11RenderTargetView*> RTVs;
		int GBufferSize = 8;//최대 8개 밖에 안됨
		RTVs.reserve(GBufferSize);

		RTVs.push_back(m_AlbedoRTV.lock()->Get());
		RTVs.push_back(m_NormalRTV.lock()->Get());
		RTVs.push_back(m_PositionRTV.lock()->Get());
		RTVs.push_back(m_DepthRTV.lock()->Get());
		RTVs.push_back(m_MetalicRoughnessRTV.lock()->Get());
		RTVs.push_back(m_AORTV.lock()->Get());
		RTVs.push_back(m_LightMapRTV.lock()->Get());
		RTVs.push_back(m_EmissiveRTV.lock()->Get());


		Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView.lock()->Get());

		Device->Context()->VSSetShader(m_InstancingVS.lock()->GetVS(), nullptr, 0);
		Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);
		Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());


		std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
		std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();
		std::shared_ptr<ConstantBuffer<MatrixPallete>>SkeletalCB = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
		std::shared_ptr<ConstantBuffer<MaterialData>> MaterialCB = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
		std::shared_ptr<ConstantBuffer<LightArray>> light = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());


		Device->Context()->VSSetConstantBuffers(1, 1, MaterialCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(1, 1, MaterialCB->GetAddress());

		//Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());

		//Device->Context()->VSSetConstantBuffers(2, 1, SkeletalCB->GetAddress());
		//Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());
	}


	if (!m_InstanceDatas.empty())
	{

		std::weak_ptr<ModelData> curModel;
		curModel = m_ResourceManager.lock()->Get<ModelData>(L"floor2_low.fbx");

		UINT strides[2];
		strides[0] = sizeof(BaseVertex);
		strides[1] = sizeof(InstanceData);

		UINT offsets[2];
		offsets[0] = 0;
		offsets[1] = 0;

		for (auto& mesh : curModel.lock()->m_Meshes)
		{
			std::shared_ptr<ConstantBuffer<MaterialData>> curMaterialData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
			std::shared_ptr<Material> curMaterial = curModel.lock()->m_Materials[mesh->m_material];
			if (curMaterial != nullptr)
			{
				MaterialData data = curMaterial->m_Data;
				//data.lightmapdata.y = curData->offset.y;
				//data.lightmapdata.z = curData->lightmapindex;
				//data.lightmapdata.w = 1; //curData->scale;
				//data.lightmaptiling = curData->tiling;
				//if (data.lightmaptiling.x != 0 || data.lightmaptiling.y != 0)
				//{
				//	data.useNEOL.w = 1;

				//	std::shared_ptr<ShaderResourceView> lightmap = m_LightManager.lock()->GetLightMap(curData->lightmapindex).lock();
				//	if (lightmap != nullptr)
				//	{
				//		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::LightMap), 1, lightmap->GetAddress());
				//	}
				//}
				curMaterialData->Update(data);

				Device->BindMaterialSRV(curMaterial);
			}

			std::vector<ID3D11Buffer*> bufferPointers[2];
			bufferPointers->push_back(mesh->VB());
			bufferPointers->push_back(m_InstanceBuffer.lock()->Get());

			m_Device.lock()->Context()->IASetVertexBuffers(0, 2, bufferPointers->data(), strides, offsets);
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			m_Device.lock()->Context()->IASetInputLayout(m_InstancingVS.lock()->InputLayout());

			m_Device.lock()->Context()->DrawIndexedInstanced(mesh->IBCount(), m_InstanceDatas.size(), 0, 0, 0);
		}

	}
	///light
	//Save GBuffer texture
	{

		std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
		std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();
		std::shared_ptr<VertexBuffer> vb = m_QuadVB.lock();
		std::shared_ptr<IndexBuffer> ib = m_QuadIB.lock();

		{
			std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"GBuffer").lock();
			std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Deferred").lock();

			Device->BindVS(m_QuadVS.lock());
			Device->Context()->PSSetShader(m_Deferred.lock()->GetPS(), nullptr, 0);

			Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
			m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

			m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, m_AlbedoSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, m_NormalSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Position), 1, m_PositionSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Depth), 1, m_DepthSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, m_MetalicRoughnessSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, m_AmbientOcclusionSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Emissive), 1, m_EmissiveSRV.lock()->GetAddress());
			Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::LightMap), 1, m_LightMapSRV.lock()->GetAddress());

			Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

			Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);

			Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
		}

	}
	/*
	*/

	m_InstanceDatas.clear();
}

void DeferredInstancing::OnResize()
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

void DeferredInstancing::SetRenderQueue(const std::vector<std::shared_ptr<RenderData>>& renderQueue)
{
	m_RenderList = renderQueue;

	//instance buffer
	for (auto& object : m_RenderList)
	{
		//벽인지
		//바닥인지
		//어떤 녀석인지 알아야 모아서 한번에 그리지
		//바리에이션이 많은데...

		//임시로
		if (object->FBX == L"floor2_low.fbx")
		{
			InstanceData temp;
			temp.world = object->world.Transpose();
			temp.worldInverse = object->world.Invert();

			m_InstanceDatas.push_back(temp);
		}
	}

	if (!m_InstanceDatas.empty())
	{

		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_Device.lock()->Context()->Map(m_InstanceBuffer.lock()->Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);

		InstanceData* dataView = reinterpret_cast<InstanceData*>(mappedData.pData);

		for (int i = 0; i < m_InstanceDatas.size(); i++)
		{
			dataView[i] = m_InstanceDatas[i];
		}
		m_Device.lock()->Context()->Unmap(m_InstanceBuffer.lock()->Get(), 0);
	}

}
