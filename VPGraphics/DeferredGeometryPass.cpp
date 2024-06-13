#include "pch.h"
#include "DeferredGeometryPass.h"

#include "ResourceManager.h"
#include "Desc.h"
#include "Material.h"
#include "Mesh.h"

#include <vector>


DeferredGeometryPass::DeferredGeometryPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{


	m_DepthStencilView = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_Main").lock();

	m_AlbedoRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Albedo").lock();
	m_NormalRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Normal").lock();
	m_PositionRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Position").lock();
	m_DepthRTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"Depth").lock();

	m_StaticMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Base").lock();
	m_SkeletalMeshVS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning").lock();
	m_GeometryPS = m_ResourceManager.lock()->Get<PixelShader>(L"MeshDeferredGeometry").lock();
}

DeferredGeometryPass::~DeferredGeometryPass()
{

}


void DeferredGeometryPass::Render()
{
	// 모든 데이터 바인딩
	// CB 업데이트 및 바인딩
	// Static, Skeletal 별로 CB 업데이트하고 Material 바인딩하고 각각 드로우
}

void DeferredGeometryPass::TestRender(
	const std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& renderList)
{
	std::shared_ptr<Device> device = m_Device.lock();

	// 렌더 타겟을 텍스처로도 사용할때
	// 텍스처를 사용한 후에 해제해주어야 하므로 텍스처를 NULL 로 설정한다.
	//TODO::추후에 어떻게 관리할지 고민해볼것
	const int num = 7;
	ID3D11ShaderResourceView* pSRV[num] = { nullptr,nullptr,nullptr,nullptr };
	device->Context()->PSSetShaderResources(0, num, pSRV);

	// Bind Common Resources
	{
		std::vector<ID3D11RenderTargetView*> RTVs;
		RTVs.reserve(GBufferSize);

		RTVs.push_back(m_AlbedoRTV->Get());
		RTVs.push_back(m_NormalRTV->Get());
		RTVs.push_back(m_PositionRTV->Get());
		RTVs.push_back(m_DepthRTV->Get());
		device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView->Get());



		device->Context()->PSSetShader(m_GeometryPS->GetPS(), nullptr, 0);
		device->Context()->PSSetSamplers(0, 1, m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock()->GetAddress());
	}

	// Mesh Update & Bind & Draw

	{
		// TODO: 너무 너무 중첩.. 정리 필요함..
		for (const auto& model : renderList)
		{
			std::shared_ptr<ModelData> curModel = model.second.second;

			for (const auto& mesh : curModel->m_Meshes)
			{
				device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

				// Static Mesh Data Update & Bind
				if (!mesh->IsSkinned())
				{
					// Shader Binding
					device->Context()->IASetInputLayout(m_StaticMeshVS->InputLayout());
					device->Context()->VSSetShader(m_StaticMeshVS->GetVS(), nullptr, 0);

					// VB & IB Binding
					device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
					device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();

					position->m_struct.world = model.second.second->world;
					position->m_struct.local = model.second.second->local;
					position->Update();	// == Bind

					// Material Binding
					if (!curModel->m_Materials.empty())
					{
						for (const auto& material : model.second.second->m_Materials)
						{
							device->Context()->PSSetShaderResources(0, 1, material->m_AlbedoSRV.lock()->GetAddress());
							device->Context()->PSSetShaderResources(1, 1, material->m_NormalSRV.lock()->GetAddress());
							//device->Context()->PSSetShaderResources(2, 1, material->m_SpecularSRV.lock()->GetAddress());
						}
					}
				}
				// Skeletal Mesh Update & Bind
				else
				{
					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// Shader Binding
					device->Context()->IASetInputLayout(m_SkeletalMeshVS->InputLayout());
					device->Context()->VSSetShader(m_SkeletalMeshVS->GetVS(), nullptr, 0);

					// VB & IB Binding
					device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
					device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

					// CB Update
					std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformData>>(L"Transform").lock();

					position->m_struct.world = curModel->world;
					position->m_struct.local = curMesh->m_node.lock()->m_World;

					position->Update();	// == Bind

					MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);
					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(model.second.second->m_name + L"MatrixPallete").lock();
					pallete->Update(matrixPallete);
					m_Device.lock()->Context()->VSSetConstantBuffers(3, 1, pallete->GetAddress());

					// Material Binding
					if (!curModel->m_Materials.empty())
					{
						for (const auto& material : model.second.second->m_Materials)
						{
							device->Context()->PSSetShaderResources(0, 1, material->m_AlbedoSRV.lock()->GetAddress());
							device->Context()->PSSetShaderResources(1, 1, material->m_NormalSRV.lock()->GetAddress());
							//m_Device->Context()->PSSetShaderResources(2, 1, material->m_SpecularSRV.lock()->GetAddress());
						}
					}
				}

				// Make a Draw Call!!
				device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}
		}
	}

	//렌더타겟 해제해줘야지 srv도 해제
	device->Context()->OMSetRenderTargets(0, nullptr, nullptr);
}
