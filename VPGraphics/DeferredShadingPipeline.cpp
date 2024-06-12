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

/// <summary>
///	생성한 패스들 초기화
/// </summary>
void DeferredShadingPipeline::Initialize(std::shared_ptr<Device>& device,
	std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height)
{
	m_DeferredLightPass->Initialize(device, resourceManager, width, height);

}

/// <summary>
///	후면 버퍼에 연결된 렌더 타겟 클리어
///	</summary>
void DeferredShadingPipeline::RenderBegin()
{
}

/// <summary>
/// 모든 패스 Render
///	패스 별 렌더 순서에 유의
/// </summary>
void DeferredShadingPipeline::Render()
{
}

/// TEST TEST TEST
void DeferredShadingPipeline::TestRender(
	const std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& renderList)
{
	m_DeferredGeometryPass->TestRender(renderList);
	m_DeferredLightPass->Render();
}

/// <summary>
///	그리기를 종료하고 화면에 출력
/// </summary>
void DeferredShadingPipeline::RenderEnd()
{
}
