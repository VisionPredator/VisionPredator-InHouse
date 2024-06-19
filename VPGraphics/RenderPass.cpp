#include "pch.h"
#include "RenderPass.h"
#include "ResourceManager.h"

#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"

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
	//m_PS = m_ResourceManager.lock()->Get<PixelShader>(L"../x64/Debug/SkinningPS.cso");
	//m_PS = m_ResourceManager.lock()->Get<PixelShader>(L"../x64/Debug/MeshPS.cso");
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
	m_Device.lock()->Context()->OMSetRenderTargets(1, m_RTV.lock()->GetAddress(), m_DSV.lock()->Get());


	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());


		for (auto& mesh : curModel->m_Meshes)
		{

			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			if (mesh->IsSkinned())
			{
				m_Device.lock()->Context()->IASetInputLayout(m_SkinningVS.lock()->InputLayout());
				m_Device.lock()->Context()->VSSetShader(m_SkinningVS.lock()->GetVS(), nullptr, 0);
				m_Device.lock()->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);
				std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
				position->m_struct.world = curModel->world;
				position->m_struct.local = curMesh->m_node.lock()->m_World;
				position->Update();

				if (!curMesh->m_BoneData.empty())
				{
					MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);

					std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(curModel->m_name + L"MatrixPallete").lock();
					pallete->Update(matrixPallete);
					m_Device.lock()->Context()->VSSetConstantBuffers(4, 1, pallete->GetAddress());
				}

				m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
				m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, position->GetAddress());

			
			}
			else
			{
				m_Device.lock()->Context()->IASetInputLayout(m_StaticVS.lock()->InputLayout());
				m_Device.lock()->Context()->VSSetShader(m_StaticVS.lock()->GetVS(), nullptr, 0);
				m_Device.lock()->Context()->PSSetShader(m_MeshPS.lock()->GetPS(), nullptr, 0);

				std::shared_ptr<ConstantBuffer<TransformData>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformData>>(L"Transform").lock();
				position->m_struct.world = curModel->world;
				position->m_struct.local = curModel->local;
				position->Update();

				m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
				m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, position->GetAddress());
			}

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{
				std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();
				std::shared_ptr<ConstantBuffer<MaterialData>> curData = m_ResourceManager.lock()->Get<ConstantBuffer<MaterialData>>(L"MaterialData").lock();

				//material이 일단 2개 인 경우가 거의 없으니까 일단 이렇게하자
				for (auto& curMaterial : curModel->m_Materials)
				{
					MaterialData curMaterialData = curMaterial->m_Data;
					curData->Update(curMaterialData);

					m_Device.lock()->Context()->PSSetSamplers(0, 1, linear->GetAddress());

					if (curMaterialData.AMRO.x > 0)
					{
						m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial->m_AlbedoSRV.lock()->GetAddress()));

					}
					
					m_Device.lock()->Context()->PSSetShaderResources(1, 1, curMaterial->m_NormalSRV.lock()->GetAddress());

					if (curMaterialData.AMRO.y > 0)
					{
						m_Device.lock()->Context()->PSSetShaderResources(4, 1, curMaterial->m_MetalicSRV.lock()->GetAddress());
					}

					if (curMaterialData.AMRO.z > 0)
					{
						m_Device.lock()->Context()->PSSetShaderResources(5, 1, curMaterial->m_RoughnessSRV.lock()->GetAddress());
					}

					if (curMaterialData.AMRO.w > 0)
					{
						m_Device.lock()->Context()->PSSetShaderResources(6, 1, curMaterial->m_AOSRV.lock()->GetAddress());
					}
				}


			}
			///Draw
			m_Device.lock()->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
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
	m_state = PassState::Static;
}

DebugPass::~DebugPass()
{

}

void DebugPass::Render()
{
	m_Device.lock()->Context()->IASetInputLayout(m_SkinningVS.lock()->InputLayout());
	m_Device.lock()->Context()->VSSetShader(m_SkinningVS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_DebugPS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			int i = 0;

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

				std::shared_ptr<Material> curMaterial = curModel->m_Materials[i];
				std::shared_ptr<Sampler> linear = m_ResourceManager.lock()->Get<Sampler>(L"Linear").lock();

				if (true)
				{
					m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial->m_AlbedoSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(0, 1, linear->GetAddress());

					m_Device.lock()->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(1, 1, linear->GetAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device.lock()->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}
		m_RenderModelQueue.pop();
	}
}
