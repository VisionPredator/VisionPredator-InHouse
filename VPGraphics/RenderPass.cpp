#include "pch.h"
#include "RenderPass.h"
#include "ResourceManager.h"

#include "ModelData.h"
#include "Mesh.h"
#include "Material.h"

RenderPass::RenderPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manger) : m_Device(device), m_ResourceManager(manger), m_VS(), m_PS(), m_RS()
{

}

RenderPass::~RenderPass()
{
	m_VS.reset();
	m_PS.reset();
	m_RS.reset();
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

void RenderPass::StaticRender()
{




}



ForwardPass::ForwardPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, D3D11_VIEWPORT* vp) : RenderPass(device, manager), m_VP(vp)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_1");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager.lock()->Get<PixelShader>(L"../x64/Debug/BasePS.cso");
	m_VS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	//m_RS = ;

	m_state = PassState::Static;

}

ForwardPass::~ForwardPass()
{

}

void ForwardPass::Render()
{
}

void ForwardPass::StaticRender()
{
	m_Device.lock()->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	m_Device.lock()->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_PS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			int i = 0;

			std::shared_ptr<ConstantBuffer<TransformCB>> transform = m_ResourceManager.lock()->Create<ConstantBuffer<TransformCB>>(L"Transform").lock();
			transform->m_struct.world = curModel->world;
			transform->m_struct.local = curModel->local;
			transform->Update();

			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(mesh->m_primitive);


			m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, transform->GetAddress());
			//m_Device.lock()->Context()->PSSetConstantBuffers(1, 1, transform->GetAddress());

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				std::shared_ptr<Material> curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV.lock()->GetSamplerAddress());

					m_Device.lock()->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV.lock()->GetSamplerAddress());

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

void ForwardPass::SkinnedRender()
{
	FLOAT Black[4] = { 0.f,0.f,0.f,1.f };
	m_Device.lock()->BeginRender(m_RTV.lock()->Get(), m_DSV.lock()->Get(), Black);

	m_Device.lock()->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	m_Device.lock()->Context()->RSSetState(m_RS.lock()->Get());
	m_Device.lock()->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_PS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();

		for (auto& mesh : curModel->m_Meshes)
		{
			std::shared_ptr<SkinnedMesh> curMesh = std::dynamic_pointer_cast<SkinnedMesh>(mesh);

			int i = 0;

			std::weak_ptr <ConstantBuffer<TransformCB>> position = m_ResourceManager.lock()->Get<ConstantBuffer<TransformCB>>(L"Transform");
			position.lock()->m_struct.world = curModel->world;
			position.lock()->m_struct.local = curMesh->m_node.lock()->m_World;
			position.lock()->Update();

			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(mesh->m_primitive);


			std::weak_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(L"MatrixPallete");
			pallete.lock()->m_struct = *(curMesh->Matrix_Pallete);
			pallete.lock()->Update();
			m_Device.lock()->Context()->VSSetConstantBuffers(3, 1, pallete.lock()->GetAddress());


			m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, position.lock()->GetAddress());
			//m_Device.lock()->Context()->PSSetConstantBuffers(0, 1, position.lock()->GetAddress());


			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				std::weak_ptr<Material> curMaterial = curModel->m_Materials[i];

				if (true)
				{
					m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial.lock()->m_DiffuseSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(0, 1, curMaterial.lock()->m_DiffuseSRV.lock()->GetSamplerAddress());

					m_Device.lock()->Context()->PSSetShaderResources(1, 1, (curMaterial.lock()->m_NormalSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(1, 1, curMaterial.lock()->m_NormalSRV.lock()->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device.lock()->Context()->DrawIndexed(curMesh->IBCount(), 0, 0);


			//그렸으면 queue에서 빼기 어디서 매프레임마다 넣어줄거니?
		}
		m_RenderModelQueue.pop();
	}

	m_Device.lock()->Context()->OMSetRenderTargets(1, m_RTV.lock()->GetAddress(), m_DSV.lock()->Get());
	m_Device.lock()->Context()->RSSetViewports(1, m_VP);
	m_Device.lock()->EndRender();
}





SkinnigPass::SkinnigPass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> manager, D3D11_VIEWPORT* vp) : RenderPass(device, manager), m_VP(vp)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_1");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager.lock()->Get<PixelShader>(L"../x64/Debug/SkinningPS.cso");
	m_VS = m_ResourceManager.lock()->Get<VertexShader>(L"Skinning");

	m_state = PassState::Skinning;


}

SkinnigPass::~SkinnigPass()
{
	m_RTV.reset();
	m_DSV.reset();
}

void SkinnigPass::Render()
{
	m_Device.lock()->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	m_Device.lock()->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_PS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			std::shared_ptr<SkinnedMesh> curMesh =  std::dynamic_pointer_cast<SkinnedMesh>(mesh);

			int i = 0;

			std::shared_ptr<ConstantBuffer<TransformCB>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformCB>>(L"Transform").lock();
			position->m_struct.world = curModel->local;
			position->m_struct.local = curMesh->m_node.lock()->m_World;
			position->Update();

			m_Device.lock()->Context()->IASetVertexBuffers(0, 1, mesh->GetAddressVB(), mesh->VBSize(), mesh->VBOffset());
			m_Device.lock()->Context()->IASetIndexBuffer(mesh->IB(), DXGI_FORMAT_R32_UINT, 0);
			m_Device.lock()->Context()->IASetPrimitiveTopology(mesh->m_primitive);

			if (!curMesh->m_BoneData.empty())
			{
				MatrixPallete matrixPallete = *(curMesh->Matrix_Pallete);

				std::shared_ptr<ConstantBuffer<MatrixPallete>> pallete = m_ResourceManager.lock()->Get<ConstantBuffer<MatrixPallete>>(curModel->m_name + L"MatrixPallete").lock();
				pallete->Update(matrixPallete);
				m_Device.lock()->Context()->VSSetConstantBuffers(3, 1,pallete->GetAddress());
			}

			m_Device.lock()->Context()->VSSetConstantBuffers(1, 1, position->GetAddress());
			m_Device.lock()->Context()->PSSetConstantBuffers(0, 1, position->GetAddress());

			// 텍스처와 샘플러를 셰이더에 바인딩
			if (!curModel->m_Materials.empty())
			{

				std::shared_ptr<Material> curMaterial = curModel->m_Materials[i];


				//매번 분기를 타면서 어떤 srv가 있는지 확인할건가?
				if (true)
				{
					m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV.lock()->GetSamplerAddress());

					m_Device.lock()->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV.lock()->GetSamplerAddress());

					//m_device->Context()->PSSetShaderResources(1, 1, (m_Materials[i]->m_SpecularSRV->GetAddress()));
					//m_device->Context()->PSSetSamplers(1, 1, m_Materials[i]->m_SpecularSRV->GetSamplerAddress());
				}
			}

			///Draw
			m_Device.lock()->Context()->DrawIndexed(mesh->IBCount(), 0, 0);
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

TexturePass::TexturePass(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager>manager, D3D11_VIEWPORT* vp) : RenderPass(device, manager), m_VP(vp)
{
	m_RTV = m_ResourceManager.lock()->Get<RenderTargetView>(L"RTV_1");
	m_DSV = m_ResourceManager.lock()->Get<DepthStencilView>(L"DSV_1");
	m_PS = m_ResourceManager.lock()->Get<PixelShader>(L"../x64/Debug/TexturePS.cso");
	m_VS = m_ResourceManager.lock()->Get<VertexShader>(L"Base");
	m_state = PassState::Texture;

}

TexturePass::~TexturePass()
{
	m_RTV.reset();
	m_DSV.reset();
}

void TexturePass::Render()
{

}

void TexturePass::StaticRender()
{
	m_Device.lock()->Context()->IASetInputLayout(m_VS.lock()->InputLayout());
	m_Device.lock()->Context()->VSSetShader(m_VS.lock()->GetVS(), nullptr, 0);
	m_Device.lock()->Context()->PSSetShader(m_PS.lock()->GetPS(), nullptr, 0);

	while (!m_RenderModelQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderModelQueue.front().lock();
		m_Device.lock()->Context()->RSSetState(curModel->RS.lock()->Get());

		for (auto& mesh : curModel->m_Meshes)
		{
			int i = 0;

			std::shared_ptr<ConstantBuffer<TransformCB>> position = m_ResourceManager.lock()->Create<ConstantBuffer<TransformCB>>(L"Transform").lock();
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

				if (true)
				{
					m_Device.lock()->Context()->PSSetShaderResources(0, 1, (curMaterial->m_DiffuseSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(0, 1, curMaterial->m_DiffuseSRV.lock()->GetSamplerAddress());

					m_Device.lock()->Context()->PSSetShaderResources(1, 1, (curMaterial->m_NormalSRV.lock()->GetAddress()));
					m_Device.lock()->Context()->PSSetSamplers(1, 1, curMaterial->m_NormalSRV.lock()->GetSamplerAddress());

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

void TexturePass::SkinnedRender()
{

}
