#pragma once
#include "MeshFilter.h"

#include "TransparencyPass.h"
#include "DebugPass.h"
#include "DeferredPass.h"
#include "OutlineAddPass.h"
#include "OutlineBlurPass.h"
#include "OutlineEdgeDetectPass.h"

/// <summary>
///	모든 패스들을 관리하는 매니저.
/// 디퍼드 렌더링 기법을 사용한다.
/// </summary>

class ParticlePass;
class TimeManager;
class ParticleManager;
class ModelData;
class DebugDrawManager;
class UIManager;

class PassManager
{
public:
	PassManager();
	~PassManager();

	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resource, const std::shared_ptr<DebugDrawManager>& debug,
		const std::shared_ptr<ParticleManager>& particleManager, const std::shared_ptr<UIManager>& uiManager, const std::shared_ptr<LightManager>& lightmanager);
	void Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList);

	void Render();
	void OnResize();

private:
	void CheckPassState(std::shared_ptr<RenderData>& model, PassState pass);
	void DrawGBuffer();
	void DrawIMGUI();

private:
	std::unordered_map<PassState,std::shared_ptr<RenderPass>> m_Passes;

	std::shared_ptr<ParticlePass> m_ParticlePass;
	std::shared_ptr<class UIPass> m_UIPass;
	std::shared_ptr<OutlineEdgeDetectPass> m_OutlineEdgeDetectPass;
	std::shared_ptr<OutlineBlurPass> m_OutlineBlurPass;
	std::shared_ptr<OutlineAddPass> m_OutlineAddPass;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
	std::weak_ptr<DebugDrawManager> m_DebugDrawManager;
	std::shared_ptr<ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager = nullptr;
	std::shared_ptr<UIManager> m_UIManager;

	VPMath::Matrix m_View;
	VPMath::Matrix m_Proj;
};

