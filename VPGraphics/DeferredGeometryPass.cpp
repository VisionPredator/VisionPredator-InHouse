#include "pch.h"
#include "DeferredGeometryPass.h"

#include "ResourceManager.h"
#include "Desc.h"
#include "Material.h"
#include "Mesh.h"

#include <vector>


void DeferredGeometryPass::Initialize(std::shared_ptr<Device>& device,
                                      std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height)
{
	m_Device = device;
	m_ResourceManager = resourceManager;

	m_Viewport.Width = static_cast<FLOAT>(width);
	m_Viewport.Height = static_cast<FLOAT>(height);

	m_DepthStencilView = std::make_shared<DepthStencilView>(device, DepthStencilViewType::Default, width, height);

	m_AlbedoRTV = std::make_shared<RenderTargetView>(device, RenderTargetViewType::OffScreen, width, height);
	m_NormalRTV = std::make_shared<RenderTargetView>(device, RenderTargetViewType::OffScreen, width, height);
	m_PositionRTV = std::make_shared<RenderTargetView>(device, RenderTargetViewType::OffScreen, width, height);
	m_DepthRTV = std::make_shared<RenderTargetView>(device, RenderTargetViewType::OffScreen, width, height);

	D3D_SHADER_MACRO macro[] =
	{
		{"SKINNING",""}, // 매크로 이름과 값을 설정
		{nullptr, nullptr}    // 배열의 끝을 나타내기 위해 nullptr로 끝낸다.
	};
	m_StaticMeshVS = std::make_shared<VertexShader>(device, L"../../../VPGraphics/MeshVS.hlsl", nullptr, -1);
	m_SkeletalMeshVS = std::make_shared<VertexShader>(device, L"../../../VPGraphics/MeshVS.hlsl", macro , -1);
	m_GeometryPS = std::make_shared<PixelShader>(device, L"MeshDeferredGeometry");

	m_ModelTransformCB = std::make_shared<ConstantBuffer<TransformData>>(device, BufferDESC::Constant::DefaultTransform);
	m_BoneTransformCB = std::make_shared<ConstantBuffer<MatrixPallete>>(device, BufferDESC::Constant::DefaultMatrixPallete);

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
	// Clear
	{
		// 렌더 타겟을 텍스처로도 사용할때
		// 텍스처를 사용한 후에 해제해주어야 하므로 텍스처를 NULL 로 설정한다.
		ID3D11ShaderResourceView* pSRV = NULL;
		m_Device->Context()->PSSetShaderResources(0, 1, &pSRV);

		m_Device->Context()->ClearDepthStencilView(m_DepthStencilView->Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		const DirectX::SimpleMath::Color white = { 1.f, 1.f, 1.f, 1.f };
		const DirectX::SimpleMath::Color black = { 0.f, 0.f, 0.f, 1.f };
		m_Device->Context()->ClearRenderTargetView(m_AlbedoRTV->Get(), white);
		m_Device->Context()->ClearRenderTargetView(m_NormalRTV->Get(), black);
		m_Device->Context()->ClearRenderTargetView(m_PositionRTV->Get(), black);
		m_Device->Context()->ClearRenderTargetView(m_DepthRTV->Get(), black);
	}

	// Bind Common Resources
	{
		std::vector<ID3D11RenderTargetView*> RTVs;
		RTVs.reserve(GBufferSize);

		RTVs.push_back(m_AlbedoRTV->Get());
		RTVs.push_back(m_NormalRTV->Get());
		RTVs.push_back(m_PositionRTV->Get());
		RTVs.push_back(m_DepthRTV->Get());
		m_Device->Context()->OMSetRenderTargets(GBufferSize, RTVs.data(), m_DepthStencilView->Get());

		m_Device->Context()->RSSetViewports(1, &m_Viewport);
		m_Device->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_Device->Context()->VSSetConstantBuffers(1, 1, m_ModelTransformCB->GetAddress());
		m_Device->Context()->VSSetConstantBuffers(3, 1, m_BoneTransformCB->GetAddress());
		// 이거 왜 슬롯이 3..
		// 슬롯이 이상해~

		m_Device->Context()->PSSetShader(m_GeometryPS->GetPS(), nullptr, 0);
		m_Device->Context()->PSSetSamplers(0, 1, m_ResourceManager->Get<Sampler>(L"Linear").lock()->GetAddress());
	}

	// Mesh Update & Bind & Draw
	{
		// TODO: 너무 너무 중첩.. 정리 필요함..
		for (const auto& model : renderList)
		{
			for (const auto& mesh : model.second.second->m_Meshes)
			{
				// Static Mesh Data Update & Bind
				if (!mesh->IsSkinned())
				{
					// Shader Binding
					m_Device->Context()->IASetInputLayout(m_StaticMeshVS->InputLayout());
					m_Device->Context()->VSSetShader(m_StaticMeshVS->GetVS(), nullptr, 0);

					// VB & IB Binding
					m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
					m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

					// CB Update
					m_ModelTransformCB->m_struct.world = model.second.second->world;
					m_ModelTransformCB->m_struct.local = model.second.second->local;
					m_ModelTransformCB->Update();	// == Bind

					// Material Binding
					if (!model.second.second->m_Materials.empty())
					{
						for (const auto& material : model.second.second->m_Materials)
						{
							m_Device->Context()->PSSetShaderResources(0, 1, material->m_DiffuseSRV.lock()->GetAddress());
							m_Device->Context()->PSSetShaderResources(1, 1, material->m_NormalSRV.lock()->GetAddress());
							m_Device->Context()->PSSetShaderResources(2, 1, material->m_SpecularSRV.lock()->GetAddress());
						}
					}
				}
				// Skeletal Mesh Update & Bind
				else
				{
					std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

					// Shader Binding
					m_Device->Context()->IASetInputLayout(m_SkeletalMeshVS->InputLayout());
					m_Device->Context()->VSSetShader(m_SkeletalMeshVS->GetVS(), nullptr, 0);

					// VB & IB Binding
					m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
					m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);

					// CB Update
					m_ModelTransformCB->m_struct.world = model.second.second->world;
					m_ModelTransformCB->m_struct.local = curMesh->m_node.lock()->m_World;
					m_ModelTransformCB->Update();	// == Bind
					m_BoneTransformCB->Update(*curMesh->Matrix_Pallete);

					// Material Binding
					if (!model.second.second->m_Materials.empty())
					{
						for (const auto& material : model.second.second->m_Materials)
						{
							m_Device->Context()->PSSetShaderResources(0, 1, material->m_DiffuseSRV.lock()->GetAddress());
							m_Device->Context()->PSSetShaderResources(1, 1, material->m_NormalSRV.lock()->GetAddress());
							//m_Device->Context()->PSSetShaderResources(2, 1, material->m_SpecularSRV.lock()->GetAddress());
						}
					}
				}

				// Make a Draw Call!!
				m_Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
			}
		}
	}
}
