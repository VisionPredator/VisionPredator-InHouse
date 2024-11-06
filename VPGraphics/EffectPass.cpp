#include "pch.h"
#include "EffectPass.h"
#include "ResourceManager.h"
#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"
#include "ConstantBuffer.h"

#include "Slot.h"

EffectPass::EffectPass(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{
	m_DepthStencilView = resourceManager->Get<DepthStencilView>(L"DSV_Deferred").lock();

	m_AlbedoRTV = resourceManager->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = resourceManager->Get<RenderTargetView>(L"Normal").lock();
	m_DepthRTV = resourceManager->Get<RenderTargetView>(L"Depth").lock();
	m_MetalicRoughnessRTV = resourceManager->Get<RenderTargetView>(L"Metalic_Roughness").lock();
	m_GBuffer = resourceManager->Get<RenderTargetView>(L"GBuffer").lock();

	m_AlbedoSRV = resourceManager->Get<ShaderResourceView>(L"Albedo").lock();
	m_NormalSRV = resourceManager->Get<ShaderResourceView>(L"Normal").lock();
	m_PositionSRV = resourceManager->Get<ShaderResourceView>(L"Position").lock();
	m_DepthSRV = resourceManager->Get<ShaderResourceView>(L"Depth").lock();
	m_MetalicRoughnessSRV = resourceManager->Get<ShaderResourceView>(L"Metalic_Roughness").lock();
	m_AmbientOcclusionSRV = resourceManager->Get<ShaderResourceView>(L"AO").lock();
	m_EmissiveSRV = resourceManager->Get<ShaderResourceView>(L"Emissive").lock();
	m_GBufferSRV = resourceManager->Get<ShaderResourceView>(L"GBuffer").lock();

	m_StaticMeshVS = resourceManager->Get<VertexShader>(L"Base").lock();
	m_PuchPS = resourceManager->Get<PixelShader>(L"MeshDeferredGeometry").lock();
	//m_TimeCB = m_ResourceManager.lock()->Create<ConstantBuffer<VPMath::XMFLOAT4>>(L"Time");

}

EffectPass::~EffectPass()
{
	//m_ResourceManager.lock()->Erase<ConstantBuffer<VPMath::XMFLOAT4>>(L"Time");
}

void EffectPass::Render(float deltaTime)
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ConstantBuffer<CameraData>> CameraCB = m_ResourceManager.lock()->Get<ConstantBuffer<CameraData>>(L"Camera").lock();
	std::shared_ptr<ConstantBuffer<TransformData>> TransformCB = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

	std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"LinearClamp").lock();
	std::shared_ptr<Sampler> linearWrap = m_ResourceManager.lock()->Get<Sampler>(L"LinearWrap").lock();

	//bind
	Device->UnBindSRV();
	Device->Context()->VSSetConstantBuffers(0, 1, CameraCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(0, 1, CameraCB->GetAddress());

	//rtv
	std::vector<ID3D11RenderTargetView*> RTVs;
	RTVs.push_back(m_GBuffer.lock()->Get());
	Device->Context()->OMSetRenderTargets(RTVs.size(), RTVs.data(), m_DepthStencilView.lock()->Get());

	//set primitive
	m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_Device.lock()->Context()->RSSetState(m_ResourceManager.lock()->Get<RenderState>(L"Solid").lock()->Get());


	//set vs, ps
	m_Device.lock()->Context()->VSSetShader(m_StaticMeshVS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_PuchPS.lock()->GetPS(), nullptr, 0);

	//set sampler
	m_Device.lock()->Context()->PSSetSamplers(0, 1, linear->GetAddress());

	//set cb
	Device->Context()->VSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());

	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Camera), 1, CameraCB->GetAddress());
	Device->Context()->PSSetConstantBuffers(static_cast<UINT>(Slot_B::Transform), 1, TransformCB->GetAddress());

	//m_TimeCB.lock()->Update({deltaTime,0,0,0});
	//Device->Context()->PSSetConstantBuffers(2, 1, m_TimeCB.lock()->GetAddress());

	//set srv
	m_Device.lock()->Context()->PSSetShaderResources(0, 1, m_PositionSRV.lock()->GetAddress());

	//render
	for (const auto& curData : m_RenderList)
	{
		std::shared_ptr<ModelData> curModel = m_ResourceManager.lock()->Get<ModelData>(curData->FBX).lock();

		if (curModel != nullptr)
		{
			for (const auto& mesh : curModel->m_Meshes)
			{
				//set vb(instance buffer)
				Device->BindMeshBuffer(mesh);

				// Static Mesh Data Update & Bind
				if (!mesh->IsSkinned())
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
					
					// 텍스처와 샘플러를 셰이더에 바인딩
					if (!curModel->m_Materials.empty())
					{
						std::shared_ptr<ConstantBuffer<MaterialData>> curMaterialData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();
						std::shared_ptr<Material> curMaterial = curModel->m_Materials[mesh->m_material];

						if (curMaterial != nullptr)
						{
							Device->Context()->PSSetSamplers(0, 1, linear->GetAddress());

							Device->BindMaterialSRV(curMaterial);
						}
					}

					Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
				}
			}
		}
	}

	//렌더타겟 해제해줘야지
	Device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
}

void EffectPass::OnResize()
{

}
