#include "pch.h"
#include "ForwardPass.h"
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

ForwardPass::ForwardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager) : RenderPass(device, manager)
{
	//m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"IMGUI");
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_Main");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main");

	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");
	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");

	m_MeshPS = m_ResourceManager.lock()->Get<PixelShader>(L"Mesh");
	m_DebugPS = m_ResourceManager.lock()->Get<PixelShader>(L"Base");

	m_state = PassState::Forward;
}

ForwardPass::~ForwardPass()
{
	m_RTV.reset();
	m_DSV.reset();
}

void ForwardPass::Render()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<DepthStencilView> DSV = m_DSV.lock();
	std::shared_ptr<RenderTargetView> RTV = m_RTV.lock();
	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

	Device->UnBindSRV();
	Device->Context()->OMSetRenderTargets(1, RTV->GetAddress(), DSV->Get());

	while (!m_RenderDataQueue.empty())
	{
		std::shared_ptr<RenderData> curData = m_RenderDataQueue.front().lock();
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();
		if (curModel != nullptr)
		{
			Device->Context()->RSSetState(curModel->RS.lock()->Get());

			int materialindex = 0; //mesh의 수와 같음

			for (auto& mesh : curModel->m_Meshes)
			{
				Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
				Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
				Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);
				Device->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

				if (mesh->IsSkinned())
				{
					Device->BindVS(m_SkeletalMeshVS.lock());

					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					TransformData renew;
					XMStoreFloat4x4(&renew.world, XMMatrixTranspose(curData->world));
					renew.local = curMesh->m_node.lock()->m_World;
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
				else
				{
					BindStatic(curData);


					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
				}

				if (!curModel->m_Materials.empty())
				{
					std::shared_ptr<Material> curMaterial = curModel->m_Materials[materialindex];
					std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();

					MaterialData curMaterialData = curMaterial->m_Data;
					curData->Update(curMaterialData);

					Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());
					{
						Device->BindMaterialSRV(curMaterial);
					}
				}

				materialindex++;

				///Draw
				Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}

		}
		m_RenderDataQueue.pop();
	}
}
