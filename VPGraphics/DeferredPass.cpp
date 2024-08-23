#include "pch.h"
#include "DeferredPass.h"
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


DeferredPass::DeferredPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	m_DepthStencilView = manager->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = manager->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = manager->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = manager->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = manager->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRoughnessRTV = manager->Get<RenderTargetView>(L"Metalic_Roughness").lock();
	m_AORTV = manager->Get<RenderTargetView>(L"AO").lock();
	m_EmissiveRTV = manager->Get<RenderTargetView>(L"Emissive").lock();
	m_LightMapRTV = manager->Get<RenderTargetView>(L"LightMap").lock();

	m_StaticMeshVS = manager->Get<VertexShader>(L"Base").lock();
	m_StaticMeshVS = manager->Get<VertexShader>(L"Skinning").lock();
	m_GeometryPS = manager->Get<PixelShader>(L"MeshDeferredGeometry").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_QuadPS = manager->Get<PixelShader>(L"Quad");

	m_Deferred = manager->Get<PixelShader>(L"MeshDeferredLight");

	m_Albedo = manager->Get<ShaderResourceView>(L"Albedo").lock();
	m_Normal = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_Position = manager->Get<ShaderResourceView>(L"Position").lock();
	m_Depth = manager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughness = manager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AO = manager->Get<ShaderResourceView>(L"AO").lock();
	m_Emissive = manager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBuffer = manager->Get<ShaderResourceView>(L"GBuffer").lock();


	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"MeshDeferredGeometry");

	//??
	m_States = std::make_unique<CommonStates>(device->Get());
}

DeferredPass::~DeferredPass()
{
	if (!m_AlbedoRTV.expired())
	{
		int a = 3;
	}
}

void DeferredPass::Render()
{
	//m_Device.lock()->Context()->OMSetDepthStencilState(m_States->DepthDefault(), 0);
	//m_Device.lock() ->Context()->RSSetState(m_States->CullNone());

	Geometry();
	Light();
}

void DeferredPass::OnResize()
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

	m_StaticMeshVS = manager->Get<VertexShader>(L"Base").lock();
	m_StaticMeshVS = manager->Get<VertexShader>(L"Skinning").lock();
	m_GeometryPS = manager->Get<PixelShader>(L"MeshDeferredGeometry").lock();

	m_QuadVB = manager->Get<VertexBuffer>(L"Quad_VB");
	m_QuadIB = manager->Get<IndexBuffer>(L"Quad_IB");
	m_QuadVS = manager->Get<VertexShader>(L"Quad");
	m_QuadPS = manager->Get<PixelShader>(L"Quad");

	m_Deferred = manager->Get<PixelShader>(L"MeshDeferredLight");

	m_Albedo = manager->Get<ShaderResourceView>(L"Albedo").lock();
	m_Normal = manager->Get<ShaderResourceView>(L"Normal").lock();
	m_Position = manager->Get<ShaderResourceView>(L"Position").lock();
	m_Depth = manager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughness = manager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AO = manager->Get<ShaderResourceView>(L"AO").lock();
	m_Emissive = manager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBuffer = manager->Get<ShaderResourceView>(L"GBuffer").lock();
	m_LightMap = manager->Get<ShaderResourceView>(L"LightMap").lock();


	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"MeshDeferredGeometry");
}

void DeferredPass::Geometry()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

	// Bind Common Resources
	{
		Device->UnBindSRV();
		std::vector<ID3D11RenderTargetView*> RTVs;
		int GBufferSize = 8;//static_cast<int>(Slot_T::End) - 1;//최대 8개 밖에 안됨
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

		Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);
		Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());


		std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
		std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();
		std::shared_ptr<ConstantBuffer<MatrixPallete>>SkeletalCB = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
		std::shared_ptr<ConstantBuffer<MaterialData>> MaterialCB = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
		std::shared_ptr<ConstantBuffer<LightArray>> light = m_ResourceManager.lock()->Get<ConstantBuffer<LightArray>>(L"LightArray").lock();

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Material), 1, MaterialCB->GetAddress());

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::LightArray), 1, light->GetAddress());

		Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());
		Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, SkeletalCB->GetAddress());

		
	}

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();

		//어떤 라이트맵을 쓸건지 오브젝트에따라 다를 수 있음 -인덱스로 추적해야할듯
		//임시로 일단 하드코딩
		std::shared_ptr<ShaderResourceView> lightmap = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Lightmap-0_comp_light.png").lock();
		//std::shared_ptr<ShaderResourceView> lightmap = m_ResourceManager.lock()->Get<ShaderResourceView>(L"indirect_Lightmap-0_comp_light.png").lock();
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::LightMap), 1, lightmap->GetAddress());

		if (curModel != nullptr)
		{
			for (const auto& mesh : curModel->m_Meshes)
			{
				Device->BindMeshBuffer(mesh);

				// Static Mesh Data Update & Bind
				//if (!mesh->IsSkinned())
				if (curData->Filter == MeshFilter::Static)
				{

					Device->BindVS(m_StaticMeshVS.lock());

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.local, XMMatrixTranspose(curData->world));
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					XMStoreFloat4x4(&renew.localInverse, (curData->world.Invert()));//전치 해주지말자 회전의 역행렬은 전치행렬임
					XMStoreFloat4x4(&renew.worldInverse, (curData->world.Invert()));//전치 해주지말자 회전의 역행렬은 전치행렬임
					position->Update(renew);	// == Bind
				}

				// Skeletal Mesh Update & Bind
				if (curData->Filter == MeshFilter::Skinning)
				{
					Device->BindVS(m_SkeletalMeshVS.lock());

					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));

					if (curMesh->m_node.lock() != nullptr)
					{
						renew.local = curMesh->m_node.lock()->m_World;
					}
					else
					{
						renew.local = VPMath::Matrix::Identity;
					}

					XMStoreFloat4x4(&renew.localInverse, (renew.local.Invert()));
					XMStoreFloat4x4(&renew.worldInverse, (renew.world.Invert()));

					position->Update(renew);
					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete;
					if (!curData->curAnimation.empty() && curData->isPlay)
					{
						std::wstring id = std::to_wstring(curData->EntityID);
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(id).lock();
					}
					else
					{
						pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete").lock();
					}
					pallete->Update();
					Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());
				}

				// 텍스처와 샘플러를 셰이더에 바인딩
				if (!curModel->m_Materials.empty())
				{
					std::shared_ptr<ConstantBuffer<MaterialData>> curMaterialData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
					std::shared_ptr<Material> curMaterial = curModel->m_Materials[mesh->m_material];

					if (curMaterial != nullptr)
					{
						MaterialData data = curMaterial->m_Data;
						data.lightmapdata.x = curData->lightmapindex;
						data.lightmapdata.y = curData->offset.x;
						data.lightmapdata.z = curData->offset.y;
						data.lightmapdata.w = 1; //curData->scale;
						data.lightmaptiling = curData->tiling;
						if (data.lightmaptiling.x != 0 || data.lightmaptiling.y != 0)
						{
							data.useNEOL.w = 1;
						}
						curMaterialData->Update(data);

						std::shared_ptr<ShaderResourceView> lightmap = m_ResourceManager.lock()->Get<ShaderResourceView>(L"Lightmap-0_comp_light.png").lock();
						Device->Context()->VSSetShaderResources(static_cast<UINT>(Slot_T::LightMap), 1, lightmap->GetAddress());
						


						Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());

						Device->BindMaterialSRV(curMaterial);
					}
				}

				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}
		}
		m_RenderDataQueue.pop();
	}
	//렌더타겟 해제해줘야지 srv도 해제
	Device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
}

void DeferredPass::Light()
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

		Device->BindVS(m_QuadVS.lock());
		Device->Context()->PSSetShader(m_Deferred.lock()->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());
		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, m_Albedo.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, m_Normal.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Position), 1, m_Position.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Depth), 1, m_Depth.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, m_MetalicRoughness.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, m_AO.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Emissive), 1, m_Emissive.lock()->GetAddress());
		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::LightMap), 1, m_LightMap.lock()->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);

		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}
}
