#pragma once
#include "MeshFilter.h"
#include "ModelData.h"

#include <map>
#include <string>
#include <memory>
#include <queue>

#include "DebugDrawManager.h"

/// <summary>
///	���۵� ���̵����� ������ �н����� ��Ƴ��� Ŭ����
/// </summary>

class ShaderResourceView;
class RenderTargetView;
class ResourceManager;
class TimeManager;

class DeferredShadingPipeline
{
public:
	DeferredShadingPipeline();
	DeferredShadingPipeline(const std::shared_ptr<Device>& device, 
		const std::shared_ptr<ResourceManager>& resourceManager, TimeManager* timeManager);
	~DeferredShadingPipeline() = default;

	void Initialize(const std::shared_ptr<Device>& device,
		const std::shared_ptr<ResourceManager>& resourceManager,
		const std::shared_ptr<DebugDrawManager>& debugDrawManager,
		const std::shared_ptr<class ParticleManager>& particleManager,
		TimeManager* timeManager);

	void Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList);

	void Render();

private:
	std::shared_ptr<Device> m_Device;
	std::shared_ptr<ResourceManager> m_ResourceManager;
	TimeManager* m_TimeManager;
	std::shared_ptr<DebugDrawManager> m_DebugDrawManager;
	std::shared_ptr<ParticleManager> m_ParticleManager;

	// Passes
	std::shared_ptr<class DeferredGeometryPass> m_DeferredGeometryPass;
	std::shared_ptr<class DeferredLightPass> m_DeferredLightPass;
	std::shared_ptr<class DebugDrawPass> m_DebugDrawPass;
	std::shared_ptr<class ParticlePass> m_ParticlePass;

	// Multi Render Target
	enum { GBufferSize = 4 };	// ���. �ϴ� ������ �״µ� ���� ����.
	std::shared_ptr<RenderTargetView> m_AlbedoRTV;
	std::shared_ptr<RenderTargetView> m_NormalRTV;
	std::shared_ptr<RenderTargetView> m_PositionRTV;
	std::shared_ptr<RenderTargetView> m_DepthRTV;

	// Shader Resource Views
	std::shared_ptr<ShaderResourceView> m_AlbedoSRV;
	std::shared_ptr<ShaderResourceView> m_NormalSRV;
	std::shared_ptr<ShaderResourceView> m_PositionSRV;
	std::shared_ptr<ShaderResourceView> m_DepthSRV;
	std::queue<std::shared_ptr<RenderData>> m_RenderQueue;
};
