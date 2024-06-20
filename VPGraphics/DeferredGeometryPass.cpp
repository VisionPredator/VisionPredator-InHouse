#include "pch.h"
#include "DeferredGeometryPass.h"

#include "ResourceManager.h"
#include "Desc.h"
#include "Material.h"
#include "Mesh.h"
#include "Slot.h"

#include <vector>


DeferredGeometryPass::DeferredGeometryPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{

	m_DepthStencilView = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Metalic").lock();
	m_RoughnessRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Roughness").lock();
	m_AORTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"AO").lock();



	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base").lock();
	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning").lock();
	m_GeometryPS = m_ResourceManager.lock()->Get<PixelShader>(L"MeshDeferredGeometry").lock();


}

DeferredGeometryPass::~DeferredGeometryPass()
{

}


void DeferredGeometryPass::Render(const std::shared_ptr<ModelData>& model)
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

	// Bind Common Resources
	{
		Device->UnBindSRV();
		std::vector<ID3D11RenderTargetView*> RTVs;
		RTVs.reserve(GBufferSize);

		RTVs.push_back(m_AlbedoRTV->Get());
		RTVs.push_back(m_NormalRTV->Get());
		RTVs.push_back(m_PositionRTV->Get());
		RTVs.push_back(m_DepthRTV->Get());
		RTVs.push_back(m_MetalicRTV->Get());
		RTVs.push_back(m_RoughnessRTV->Get());
		RTVs.push_back(m_AORTV->Get());
		Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView->Get());

		Device->Context()->PSSetShader(m_GeometryPS->GetPS(), nullptr, 0);
		Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
	}

	// Mesh Update & Bind & Draw

	{
		std::shared_ptr<ModelData> curModel = model;


		int materialindex = 0; //mesh의 숫자와 동일

		for (const auto& mesh : curModel->m_Meshes)
		{
			Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			// Static Mesh Data Update & Bind
			if (!mesh->IsSkinned())
			{
				// Shader Binding
				Device->Context()->IASetInputLayout(m_StaticMeshVS->InputLayout());
				Device->Context()->VSSetShader(m_StaticMeshVS->GetVS(), nullptr, 0);

				// VB & IB Binding
				Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
				Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

				// CB Update
				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

				position->m_struct.world = curModel->world;
				position->m_struct.local = curModel->local;
				position->Update();	// == Bind
			}
			// Skeletal Mesh Update & Bind
			else
			{
				std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

				// Shader Binding
				Device->Context()->IASetInputLayout(m_SkeletalMeshVS->InputLayout());
				Device->Context()->VSSetShader(m_SkeletalMeshVS->GetVS(), nullptr, 0);

				// VB & IB Binding
				Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
				Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

				// CB Update
				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

				position->m_struct.world = curModel->world;
				position->m_struct.local = curMesh->m_node.lock()->m_World;

				position->Update();	// == Bind

				MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);
				std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(model->m_name + L"MatrixPallete").lock();
				pallete->Update(matrixPallete);
				m_Device.lock()->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());

			}

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{
				std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();

				std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];
				MaterialData curMaterialData = curMaterial->m_Data;
				curData->Update(curMaterialData);

				Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());

				if (curMaterialData.useAMRO.x > 0)
				{
					Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Albedo), 1, (curMaterial->m_AlbedoSRV.lock()->GetAddress()));
				}

				if (curMaterialData.useN > 0)
				{
					Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Normal), 1, curMaterial->m_NormalSRV.lock()->GetAddress());
				}

				if (curMaterialData.useAMRO.y > 0)
				{
					Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Metalic), 1, curMaterial->m_MetalicSRV.lock()->GetAddress());
				}

				if (curMaterialData.useAMRO.z > 0)
				{
					Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::Roughness), 1, curMaterial->m_RoughnessSRV.lock()->GetAddress());
				}

				if (curMaterialData.useAMRO.w > 0)
				{
					Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::AO), 1, curMaterial->m_AOSRV.lock()->GetAddress());
				}

			}

			Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

			materialindex++;

		}

		//렌더타겟 해제해줘야지 srv도 해제
		Device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
	}
}
