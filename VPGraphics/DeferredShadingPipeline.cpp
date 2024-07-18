#include "pch.h"
#include "DeferredShadingPipeline.h"

#include "Device.h"

#pragma region PASS
#include "DeferredGeometryPass.h"
#include "DeferredLightPass.h"
#include "DebugDrawPass.h"
#pragma endregion

#pragma region MANAGER
#include "DebugDrawManager.h"
#include "ParticleManager.h"
#include "ParticlePass.h"
#include "ResourceManager.h"
#include "TimeManager.h"
#pragma endregion

/// <summary>
/// 디퍼드 렌더링에서 사용할 패스 모두 생성
/// </summary>
DeferredShadingPipeline::DeferredShadingPipeline()
{
	m_DeferredGeometryPass = std::make_shared<DeferredGeometryPass>();
	m_DeferredLightPass = std::make_shared<DeferredLightPass>();
	m_DebugDrawPass = std::make_shared<DebugDrawPass>();
	m_ParticlePass = std::make_shared<ParticlePass>();
}


DeferredShadingPipeline::DeferredShadingPipeline(const std::shared_ptr<Device>& device, 
	const std::shared_ptr<ResourceManager>& resourceManager, TimeManager* timeManager)
	: m_Device(device), m_ResourceManager(resourceManager), m_TimeManager(timeManager)
{
}

void DeferredShadingPipeline::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager, 
	const std::shared_ptr<DebugDrawManager>& debugDrawManager,
	const std::shared_ptr<ParticleManager>& particleManager,
	TimeManager* timeManager)
{
	m_Device = device;
	m_ResourceManager = resourceManager;
	m_DebugDrawManager = debugDrawManager;
	m_ParticleManager = particleManager;
	m_TimeManager = timeManager;

	SimpleMath::Matrix view = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock()->m_struct.view;
	SimpleMath::Matrix proj = m_ResourceManager->Get<ConstantBuffer<CameraData>>(L"Camera").lock()->m_struct.proj;

	m_DeferredGeometryPass->Initialize(device, resourceManager, view, proj);
	m_DeferredLightPass->Initialize(device, resourceManager);
	m_DebugDrawPass->Initialize(device, debugDrawManager, resourceManager, view, proj);
	m_ParticlePass->Initialize(device, resourceManager, particleManager, timeManager, view, proj);
}

void DeferredShadingPipeline::Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList)
{
	for (auto& model : RenderList)
	{
		std::shared_ptr<RenderData> curModel = model.second;

		PassState temp = model.second->Pass;

		temp &= PassState::Deferred;
		if (temp == PassState::Deferred)
		{

		}
		m_RenderQueue.push(curModel);
	}
}

/// <summary>
/// 모든 패스 Render
///	패스 별 렌더 순서에 유의
/// </summary>
void DeferredShadingPipeline::Render()
{
	while (!m_RenderQueue.empty())
	{
		std::shared_ptr<RenderData> curModel = m_RenderQueue.front();

		m_DeferredGeometryPass->Render(curModel);


		m_RenderQueue.pop();
	}

	m_DeferredLightPass->Render();
	m_DebugDrawPass->Render();

	m_ParticlePass->Render();
}
