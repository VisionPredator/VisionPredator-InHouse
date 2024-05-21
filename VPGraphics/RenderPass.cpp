#include "pch.h"
#include "RenderPass.h"



#include "ResourceManager.h"

RenderPass::RenderPass(Device* device, ResourceManager* manger) : m_Device(device), m_ResourceManager(manger), m_VS(nullptr), m_PS(nullptr), m_RS(nullptr)
{

}

RenderPass::~RenderPass()
{

}

void RenderPass::AddModelData(ModelData* model)
{
	m_RenderModelQueue.push(model);
}

void RenderPass::AddModelData(std::map<std::wstring, std::pair<PassState, ModelData*>>& model_list)
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

void RenderPass::StaticRender()
{




}



ForwardPass::ForwardPass(Device* device, ResourceManager* manager, D3D11_VIEWPORT* vp) : RenderPass(device, manager), m_VP(vp)
{
	m_RTV = manager->Get<RenderTargetView>(L"RTV_1");
	m_DSV = manager->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager->Get<PixelShader>(L"../x64/Debug/BasePS.cso");
	m_VS = m_ResourceManager->Get<VertexShader>(L"../x64/Debug/BaseVS.cso");
	//m_RS = ;

	m_state = PassState::Base;

}

ForwardPass::~ForwardPass()
{

}

void ForwardPass::Render()
{

}

void ForwardPass::StaticRender()
{
	m_Device->Context()->IASetInputLayout(m_VS->InputLayout());
	m_Device->Context()->VSSetShader(m_VS->GetVS(), nullptr, 0);
	m_Device->Context()->PSSetShader(m_PS->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		ModelData* curModel = m_RenderModelQueue.front();
		m_Device->Context()->RSSetState(curModel->RS->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			int i = 0;

			ConstantBuffer<WorldTransformCB>* position = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"Transform");
			position->m_struct.world = curModel->world;
			position->Update();

			m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			ConstantBuffer<LocalTransformCB>* local = m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(L"Local");
			local->m_struct.local = curModel->local;
			local->Update();

			m_Device->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device->Context()->PSSetConstantBuffers(0, 1, position->GetAddress());
			m_Device->Context()->VSSetConstantBuffers(2, 1, local->GetAddress());

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				Material* curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV->GetSamplerAddress());

					m_Device->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}

		m_RenderModelQueue.pop();
	}

	
}

void ForwardPass::SkinnedRender()
{


	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	m_Device->BeginRender(m_RTV->Get(), m_DSV->Get(), Black);

	m_Device->Context()->IASetInputLayout(m_VS->InputLayout());
	m_Device->Context()->RSSetState(m_RS->Get());
	m_Device->Context()->VSSetShader(m_VS->GetVS(), nullptr, 0);
	m_Device->Context()->PSSetShader(m_PS->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		ModelData* curModel = m_RenderModelQueue.front();

		for (auto& mesh : curModel->m_Meshes)
		{
			SkinnedMesh* curMesh = dynamic_cast<SkinnedMesh*>(mesh);

			int i = 0;

			ConstantBuffer<WorldTransformCB>* position = m_ResourceManager->Get<ConstantBuffer<WorldTransformCB>>(L"Transform");
			position->m_struct.world = curModel->world;
			position->Update();

			m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			ConstantBuffer<LocalTransformCB>* local = m_ResourceManager->Get<ConstantBuffer<LocalTransformCB>>(L"Local");
			local->m_struct.local = curMesh->m_node->m_World;
			local->Update();
			m_Device->Context()->VSSetConstantBuffers(2, 1, local->GetAddress());

			ConstantBuffer<MatrixPallete>* pallete = m_ResourceManager->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete");
			pallete->m_struct = *(curMesh->Matrix_Pallete);
			pallete->Update();
			m_Device->Context()->VSSetConstantBuffers(3, 1, pallete->GetAddress());


			m_Device->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device->Context()->PSSetConstantBuffers(0, 1, position->GetAddress());
			m_Device->Context()->VSSetConstantBuffers(2, 1, local->GetAddress());


			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				Material* curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV->GetSamplerAddress());

					m_Device->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);


			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}
		m_RenderModelQueue.pop();
	}


	m_Device->Context()->OMSetRenderTargets(1, m_RTV->GetAddress(), m_DSV->Get());
	m_Device->Context()->RSSetViewports(1, m_VP);
	m_Device->EndRender();
}





SkinnigPass::SkinnigPass(Device* device, ResourceManager* manager, D3D11_VIEWPORT* vp) : RenderPass(device,manager), m_VP(vp)
{
	m_RTV = manager->Get<RenderTargetView>(L"RTV_1");
	m_DSV = manager->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager->Get<PixelShader>(L"../x64/Debug/SkinningPS.cso");
	m_VS = m_ResourceManager->Get<VertexShader>(L"../x64/Debug/SkinningVS.cso");

	m_state = PassState::Skinning;


}

SkinnigPass::~SkinnigPass()
{

}

void SkinnigPass::Render()
{
	m_Device->Context()->IASetInputLayout(m_VS->InputLayout());
	m_Device->Context()->VSSetShader(m_VS->GetVS(), nullptr, 0);
	m_Device->Context()->PSSetShader(m_PS->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		ModelData* curModel = m_RenderModelQueue.front();
		m_Device->Context()->RSSetState(curModel->RS->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			SkinnedMesh* curMesh =  dynamic_cast<SkinnedMesh*>(mesh);

			int i = 0;

			ConstantBuffer<WorldTransformCB>* position = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"Transform");
			position->m_struct.world = curModel->world;
			position->Update();

			m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			ConstantBuffer<LocalTransformCB>* local = m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(L"Local");	
			local->m_struct.local = curMesh->m_node->m_World;
			local->Update();

			if (!curMesh->m_BoneData.empty())
			{
				MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);

				ConstantBuffer<MatrixPallete>* pallete = m_ResourceManager->Get<ConstantBuffer<MatrixPallete>>(curModel->m_name + L"MatrixPallete");
				pallete->Update(matrixPallete);
				m_Device->Context()->VSSetConstantBuffers(3, 1,pallete->GetAddress());
			}



			m_Device->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device->Context()->PSSetConstantBuffers(0, 1, position->GetAddress());
			m_Device->Context()->VSSetConstantBuffers(2, 1, local->GetAddress());

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				Material* curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV->GetSamplerAddress());

					m_Device->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}

		m_RenderModelQueue.pop();
	}

}

void SkinnigPass::StaticRender()
{

}

void SkinnigPass::SkinnedRender()
{

}

TexturePass::TexturePass(Device* device, ResourceManager* manager, D3D11_VIEWPORT* vp) : RenderPass(device,manager), m_VP(vp)
{
	m_RTV = manager->Get<RenderTargetView>(L"RTV_1");
	m_DSV = manager->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager->Get<PixelShader>(L"../x64/Debug/TexturePS.cso");
	m_VS = m_ResourceManager->Get<VertexShader>(L"../x64/Debug/TextureVS.cso");
	m_state = PassState::Texture;

}

TexturePass::~TexturePass()
{

}

void TexturePass::Render()
{

}

void TexturePass::StaticRender()
{
	m_Device->Context()->IASetInputLayout(m_VS->InputLayout());
	m_Device->Context()->VSSetShader(m_VS->GetVS(), nullptr, 0);
	m_Device->Context()->PSSetShader(m_PS->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		ModelData* curModel = m_RenderModelQueue.front();
		m_Device->Context()->RSSetState(curModel->RS->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			int i = 0;

			ConstantBuffer<WorldTransformCB>* position = m_ResourceManager->Create<ConstantBuffer<WorldTransformCB>>(L"Transform");
			position->m_struct.world = curModel->world;
			position->Update();

			m_Device->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			ConstantBuffer<LocalTransformCB>* local = m_ResourceManager->Create<ConstantBuffer<LocalTransformCB>>(L"Local");
			local->m_struct.local = curModel->local;
			local->Update();

			m_Device->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device->Context()->PSSetConstantBuffers(0, 1, position->GetAddress());
			m_Device->Context()->VSSetConstantBuffers(2, 1, local->GetAddress());

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				Material* curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV->GetSamplerAddress());

					m_Device->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV->GetAddress()));
					m_Device->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device->Context()->DrawIndexed(mesh->IBCount(), 0, 0);

			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}

		m_RenderModelQueue.pop();
	}
}

void TexturePass::SkinnedRender()
{

}
