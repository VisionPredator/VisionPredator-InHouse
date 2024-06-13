#include "pch.h"
#include "DeferredShadingPipeline.h"

#include "DeferredGeometryPass.h"
#include "DeferredLightPass.h"
#include "Device.h"
#include "ResourceManager.h"
#include "RenderTargetView.h"
#include "ShaderResourceView.h"

/// <summary>
/// 디퍼드 렌더링에서 사용할 패스 모두 생성
/// </summary>
DeferredShadingPipeline::DeferredShadingPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resourceManager)
{
	m_DeferredGeometryPass = std::make_shared<DeferredGeometryPass>(device,resourceManager);
	m_DeferredLightPass = std::make_shared<DeferredLightPass>(device, resourceManager);

}

DeferredShadingPipeline::~DeferredShadingPipeline()
{

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
/// 모든 패스 Render
///	패스 별 렌더 순서에 유의
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
}

