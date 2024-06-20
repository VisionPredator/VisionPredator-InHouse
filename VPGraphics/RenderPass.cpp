#include "pch.h"
#include "RenderPass.h"
#include "ResourceManager.h"

#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#include "Slot.h"

RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger) : m_Device(device), m_ResourceManager(manger), m_SkinningVS(), m_DebugPS()
{

}

RenderPass::~RenderPass()
{
	m_SkinningVS.reset();
	m_DebugPS.reset();

	m_RTV.reset();
	m_DSV.reset();

	m_Device.reset();
	m_ResourceManager.reset();
}

void RenderPass::AddModelData(std::shared_ptr<ModelData> model)
{
	m_RenderModelQueue.push(model);
}

void RenderPass::AddModelData(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& model_list)
{
	for (auto& model : model_list)
	{
		PassState temp = model.second.first;
		temp &= m_state;
		if (temp == m_state)
		{
			m_RenderModelQueue.push(model.second.second);
		}
	}
}

FowardPass::FowardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_Main");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main");

	m_SkinningVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");

	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"Mesh");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");

	m_state = PassState::Foward;
}

FowardPass::~FowardPass()
{
	m_RTV.reset();
	m_DSV.reset();
}

void FowardPass::Render()
{

	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();


	Device->UnBindSRV();
	Device->Context()->OMSetRenderTargets(1, RTV->GetAddress(), DSV->Get());

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		Device->Context()->RSSetState(curModel->RS.lock()->Get());


		for (auto& mesh : curModel->m_Meshes)
		{
			Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			if (mesh->IsSkinned())
			{
				Device->Context()->IASetInputLayout(m_SkinningVS.lock()->InputLayout());
				Device->Context()->VSSetShader(m_SkinningVS.lock()->GetVS(), nullptr, 0);
				Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

				std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);
				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

				TransformData renew;
				renew.world = curModel->world;
				renew.local = curMesh->m_node.lock()->m_World;

				position->Update(renew);

				if (!curMesh->m_BoneData.empty())
				{
					MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);

					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(curModel->m_name + L"MatrixPallete").lock();
					pallete->Update(matrixPallete);
					Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::MatrixPallete), 1, pallete->GetAddress());
				}

				Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
				Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
			}
			else
			{
				Device->Context()->IASetInputLayout(m_StaticVS.lock()->InputLayout());
				Device->Context()->VSSetShader(m_StaticVS.lock()->GetVS(), nullptr, 0);
				Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

				TransformData renew;
				renew.world = curModel->world;
				renew.local = curModel->local;
				position->Update(renew);

				Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
				Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, position->GetAddress());
			}

			if (!curModel->m_Materials.empty())
			{
				static int materialindex = 0;

				std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
			
				std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];

				//for (auto& curMaterial : curModel->m_Materials)
				{
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

					if (curModel->m_Materials.size() < materialindex)
					{
						materialindex++;
					}
				}
			}
			///Draw
			Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
		}

		m_RenderModelQueue.pop();
	}

}

DebugPass::DebugPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_Main");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");
	m_SkinningVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_state = PassState::Debug;
}

DebugPass::~DebugPass()
{

}

void DebugPass::Render()
{

	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();


	Device->Context()->IASetInputLayout(m_SkinningVS.lock()->InputLayout());
	Device->Context()->VSSetShader(m_SkinningVS.lock()->GetVS(), nullptr, 0);
	Device->Context()->PSSetShader(m_DebugPS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());

		int materialindex = 0; //mesh의 숫자와 동일

		for (auto& mesh : curModel->m_Meshes)
		{
			std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
			position->m_struct.world = curModel->world;
			position->m_struct.local = curModel->local;
			position->Update();

			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(mesh->m_primitive);


			m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, position->GetAddress());

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
		}
		m_RenderModelQueue.pop();
	}
}
