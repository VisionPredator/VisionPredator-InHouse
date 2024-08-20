#include "pch.h"
#include "PassManager.h"

#include <memory>

#include "ModelData.h"

#pragma region Manager
#include "ResourceManager.h"
#include "DebugDrawManager.h"
#include "ParticleManager.h"
#include "TimeManager.h"
#include "UIManager.h"
#pragma endregion 

#include "ParticlePass.h"
#include "GeoMetryPass.h"
#include "UIPass.h"

#include "StaticData.h"
#include "Slot.h"

PassManager::PassManager()
	: m_ParticlePass(std::make_shared<ParticlePass>())
	, m_UIPass(std::make_shared<UIPass>())
{
}

PassManager::~PassManager()
{
	for (auto& pass : m_Passes)
	{
		pass.second.reset();
	}

	m_Passes.clear();
}

void PassManager::Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resource, const std::shared_ptr<DebugDrawManager>& debug,
	const std::shared_ptr<ParticleManager>& particleManager, const std::shared_ptr<UIManager>& uiManager)
{
	m_Device = device;
	m_ResourceManager = resource;
	m_DebugDrawManager = debug;
	m_ParticleManager = particleManager;
	m_UIManager = uiManager;

	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Debug, std::make_shared<DebugPass>(m_Device.lock(), m_ResourceManager.lock(), m_DebugDrawManager.lock())));
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Deferred, std::make_shared<DeferredPass>(m_Device.lock(), m_ResourceManager.lock())));
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::Forward, std::make_shared<ForwardPass>(m_Device.lock(), m_ResourceManager.lock())));
	m_Passes.insert(std::make_pair<PassState, std::shared_ptr<RenderPass>>(PassState::GeoMetry, std::make_shared<GeoMetryPass>(m_Device.lock(), m_ResourceManager.lock())));

	m_ParticlePass->Initialize(m_Device.lock(), m_ResourceManager.lock(), m_ParticleManager, m_TimeManager);
	m_UIPass->Initialize(m_Device.lock(), m_ResourceManager.lock(), m_UIManager);
}

void PassManager::Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList)
{
	//비트 연산으로 해보자
	for (auto& model : RenderList)
	{
		std::shared_ptr<RenderData> curModel = model.second;
		CheckPassState(curModel, PassState::Deferred);
		CheckPassState(curModel, PassState::Forward);
		CheckPassState(curModel, PassState::Debug);
		CheckPassState(curModel, PassState::GeoMetry);
	}
}

void PassManager::Render()
{
	m_Passes[PassState::Debug]->Render();
	m_Passes[PassState::GeoMetry]->Render();
	m_Passes[PassState::Deferred]->Render();

	//m_Passes[PassState::Forward]->Render();		// 필요 없는 패스
	//DrawGBuffer();		// 필요 없는 패스

	m_ParticlePass->Render();
	m_UIPass->Render();

	// 여태까지는 offscreenRTV에 그리다가 이제 여기서 backbufferRTV에 그린다.
	DrawIMGUI();
}

void PassManager::OnResize()
{
	for (auto& pass : m_Passes)
	{
		pass.second->OnResize();
	}
}

void PassManager::CheckPassState(std::shared_ptr<RenderData>& model, PassState pass)
{
	PassState temp = model->Pass;
	temp &= pass;
	if (temp == pass)
	{
		m_Passes[pass]->AddModelData(model);
	}
}

void PassManager::DrawGBuffer()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<Sampler> linear = resourcemanager->Get<Sampler>(L"Linear").lock();
	std::shared_ptr<VertexBuffer> vb = resourcemanager->Get<VertexBuffer>(L"Quad_VB").lock();
	std::shared_ptr<IndexBuffer> ib = resourcemanager->Get<IndexBuffer>(L"Quad_IB").lock();
	std::shared_ptr<PixelShader> ps = resourcemanager->Get<PixelShader>(L"Quad").lock();
	std::shared_ptr<VertexShader> vs = resourcemanager->Get<VertexShader>(L"Quad").lock();
	std::shared_ptr<ShaderResourceView> gBuffer = resourcemanager->Get<ShaderResourceView>(L"GBuffer").lock();

	std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"IMGUI").lock();
	//std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Main").lock();

	Device->UnBindSRV();
	Device->BindVS(vs);
	Device->Context()->PSSetShader(ps->GetPS(), nullptr, 0);

	Device->Context()->RSSetState(resourcemanager->Get<RenderState>(L"Solid").lock()->Get());

	m_Device.lock()->Context()->IASetVertexBuffers(0, 1, vb->GetAddress(), vb->Size(), vb->Offset());
	m_Device.lock()->Context()->IASetIndexBuffer(ib->Get(), DXGI_FORMAT_R32_UINT, 0);

	m_Device.lock()->Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Device->Context()->PSSetShaderResources(static_cast<UINT>(Slot_T::GBuffer), 1, gBuffer->GetAddress());

	Device->Context()->PSSetSamplers(static_cast<UINT>(Slot_S::Linear), 1, linear->GetAddress());

	Device->Context()->OMSetRenderTargets(1, rtv->GetAddress(), nullptr);
	Device->Context()->DrawIndexed(Quad::Index::count, 0, 0);

}

void PassManager::DrawIMGUI()
{
	std::shared_ptr<Device> Device = m_Device.lock();
	std::shared_ptr<ResourceManager> resourcemanager = m_ResourceManager.lock();
	std::shared_ptr<Sampler> linear = resourcemanager->Get<Sampler>(L"Linear").lock();
	std::shared_ptr<VertexBuffer> vb = resourcemanager->Get<VertexBuffer>(L"Quad_VB").lock();
	std::shared_ptr<IndexBuffer> ib = resourcemanager->Get<IndexBuffer>(L"Quad_IB").lock();
	std::shared_ptr<PixelShader> ps = resourcemanager->Get<PixelShader>(L"Quad").lock();
	std::shared_ptr<VertexShader> vs = resourcemanager->Get<VertexShader>(L"Quad").lock();
	std::shared_ptr<ShaderResourceView> gui = resourcemanager->Get<ShaderResourceView>(L"GBuffer").lock();
	std::shared_ptr<RenderTargetView> rtv = resourcemanager->Get<RenderTargetView>(L"RTV_Main").lock();
	std::shared_ptr<DepthStencilView> dsv = resourcemanager->Get<DepthStencilView>(L"DSV_Main").lock();

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
