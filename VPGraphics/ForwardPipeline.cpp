#include "pch.h"
#include "ForwardPipeline.h"

#include "ModelData.h"
#include "ResourceManager.h"

#include "StaticData.h"
#include "Slot.h"

ForwardPipeline::ForwardPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource) : m_Device(device), m_ResourceManager(resource)
{

}

ForwardPipeline::~ForwardPipeline()
{
	for (auto& pass : m_Passes)
	{
		pass.second.reset();
	}

	m_Passes.clear();
}

void ForwardPipeline::Initialize()
{
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Deferred, std::make_shared<DeferredPass>(m_Device.lock(), m_ResourceManager.lock())));
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Foward, std::make_shared<FowardPass>(m_Device.lock(), m_ResourceManager.lock())));
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Debug, std::make_shared<DebugPass>(m_Device.lock(), m_ResourceManager.lock())));
}

void ForwardPipeline::Update(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& RenderList)
{
	int index = 0;

	//비트 연산으로 해보자
	for (auto& model : RenderList)
	{
		PassState curState = model.second.first;
		PassState temp = curState;

		temp = curState;
		temp &= PassState::Foward;
		if (temp == PassState::Foward)
		{
			m_Passes[temp]->AddModelData(model.second.second);
		}

		temp = curState;
		temp &= PassState::Debug;
		if (temp == PassState::Debug)
		{
			m_Passes[temp]->AddModelData(model.second.second);
		}

		temp = curState;
		temp &= PassState::Deferred;
		if (temp == PassState::Deferred)
		{
			//m_Passes[temp]->AddModelData(model.second.second);
		}
	}
}

void ForwardPipeline::Render()
{

	for (auto& pass : m_Passes)
	{
		pass.second->Render();
	}


	{
		std::shared_ptr<Device> Device = m_Device.lock();
		std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
		std::shared_ptr<Sampler> linear = resourcemanager->Get<Sampler>(L"Linear").lock();
		std::shared_ptr<VertexBuffer> vb = resourcemanager->Get<VertexBuffer>(L"Quad_VB").lock();
		std::shared_ptr<IndexBuffer> ib = resourcemanager->Get<IndexBuffer>(L"Quad_IB").lock();
		std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"RTV_Main").lock();
		std::shared_ptr<PixelShader> ps = resourcemanager->Get<PixelShader>(L"Quad").lock();
		std::shared_ptr<VertexShader> vs = resourcemanager->Get<VertexShader>(L"Quad").lock();
		std::shared_ptr<ShaderResourceView> gui = m_ResourceManager.lock()->Get<ShaderResourceView>(L"IMGUI").lock();

		Device->UnBindSRV();

		Device->Context()->IASetInputLayout(vs->InputLayout());
		Device->Context()->VSSetShader(vs->GetVS(), nullptr, 0);
		Device->Context()->PSSetShader(ps->GetPS(), nullptr, 0);

		Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());

		m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
		m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

		m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::GBuffer), 1, gui->GetAddress());

		Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

		Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
		Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);
	}
}
