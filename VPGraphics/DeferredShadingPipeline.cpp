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
/// ���۵� ���������� ����� �н� ��� ����
/// </summary>
DeferredShadingPipeline::DeferredShadingPipeline()
{
	m_DeferredGeometryPass = std::make_shared<DeferredGeometryPass>();
	m_DeferredLightPass = std::make_shared<DeferredLightPass>();
	m_DebugDrawPass = std::make_shared<DebugDrawPass>();
}


void DeferredShadingPipeline::Initialize(const std::shared_ptr<Device>& device,
	const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DebugDrawManager>& debugDrawManager,
	const std::shared_ptr<Camera>& camera)
{
	m_DeferredGeometryPass->Initialize(device, resourceManager);
	m_DeferredLightPass->Initialize(device, resourceManager);
	m_DebugDrawPass->Initialize(device, debugDrawManager, resourceManager, camera);
}

void DeferredShadingPipeline::Update(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& RenderList)
{
	for (auto& model : RenderList)
	{
		PassState curState = model.second.first;
		PassState temp = curState;

		temp &= PassState::Deferred;
		if (temp == PassState::Deferred)
		{
			std::shared_ptr<ModelData> curModel = model.second.second;
			m_RenderQueue.push(curModel);
		}
	}

}

/// <summary>
/// ��� �н� Render
///	�н� �� ���� ������ ����
/// </summary>
void DeferredShadingPipeline::Render()
{
	while (!m_RenderQueue.empty())
	{
		std::shared_ptr<ModelData> curModel = m_RenderQueue.front();
		m_DeferredGeometryPass->Render(curModel);


		m_RenderQueue.pop();
	}

	m_DeferredLightPass->Render();

	m_DebugDrawPass->Render();
}

