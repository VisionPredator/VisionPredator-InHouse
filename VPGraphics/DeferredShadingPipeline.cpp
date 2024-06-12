#include "pch.h"
#include "DeferredShadingPipeline.h"

#include "DeferredGeometryPass.h"
#include "DeferredLightPass.h"
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

}

/// <summary>
///	������ �н��� �ʱ�ȭ
/// </summary>
void DeferredShadingPipeline::Initialize(std::shared_ptr<Device>& device,
	std::shared_ptr<ResourceManager>& resourceManager, const uint32_t& width, const uint32_t& height)
{
	m_DeferredGeometryPass->Initialize(device, resourceManager, width, height);
	m_DeferredLightPass->Initialize(device, resourceManager, width, height);

}

/// <summary>
///	�ĸ� ���ۿ� ����� ���� Ÿ�� Ŭ����
///	</summary>
void DeferredShadingPipeline::RenderBegin()
{
}

/// <summary>
/// ��� �н� Render
///	�н� �� ���� ������ ����
/// </summary>
void DeferredShadingPipeline::Render()
{
}

/// TEST TEST TEST
void DeferredShadingPipeline::TestRender(
	const std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& renderList)
{
	//m_DeferredGeometryPass->Render();
	m_DeferredGeometryPass->TestRender(renderList);
}

/// <summary>
///	�׸��⸦ �����ϰ� ȭ�鿡 ���
/// </summary>
void DeferredShadingPipeline::RenderEnd()
{
}