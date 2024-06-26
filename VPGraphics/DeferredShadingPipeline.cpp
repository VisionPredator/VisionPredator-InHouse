#include "pch.h"
#include "DeferredShadingPipeline.h"

#include "DebugDrawManager.h"
#include "DeferredGeometryPass.h"
#include "DeferredLightPass.h"
#include "DebugDrawPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"

/// <summary>
/// 디퍼드 렌더링에서 사용할 패스 모두 생성
/// </summary>
DeferredShadingPipeline::DeferredShadingPipeline()
{
	m_DeferredGeometryPass = std::make_shared<DeferredGeometryPass>();
	m_DeferredLightPass = std::make_shared<DeferredLightPass>();
	m_DebugDrawPass = std::make_shared<DebugDrawPass>();
}


DeferredShadingPipeline::DeferredShadingPipeline(std::shared_ptr<Device>& device, std::shared_ptr<ResourceManager>& resourceManager) : m_Device(device), m_ResourceManager(resourceManager)
{

}

void DeferredShadingPipeline::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DebugDrawManager>& debugDrawManager,
	const DirectX::SimpleMath::Matrix view, const DirectX::SimpleMath::Matrix proj)
{
	m_DeferredGeometryPass->Initialize(device, resourceManager, view, proj);
	m_DeferredLightPass->Initialize(device, resourceManager);
	m_DebugDrawPass->Initialize(device, debugDrawManager, resourceManager, view, proj);
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
}

