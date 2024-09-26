#pragma once
#include "MeshFilter.h"

#include "TransparencyPass.h"
#include "DebugPass.h"
#include "DeferredPass.h"


/// <summary>
///	모든 패스들을 관리하는 매니저.
/// 디퍼드 렌더링 기법을 사용한다.
/// </summary>

class ObjectMaskPass;
class ParticlePass;
class OutlineEdgeDetectPass;
class OutlineBlurPass;
class OutlineAddPass;
class TimeManager;
class ParticleManager;
class ModelData;
class DebugDrawManager;
class UIManager;
class GeoMetryPass;
class VPOutLinePass;
class RimLight;

class PassManager
{
public:
	PassManager();
	~PassManager();

	void Initialize(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resource, const std::shared_ptr<DebugDrawManager>& debug,
		const std::shared_ptr<ParticleManager>& particleManager, const std::shared_ptr<UIManager>& uiManager, const std::shared_ptr<LightManager>& lightmanager);
	void Update(const std::vector<std::shared_ptr<RenderData>>& afterCulling);

	void Render();
	void OnResize();
	void SetVP(bool isVP);
private:
	void DrawIMGUI();

private:
	std::vector<std::shared_ptr<RenderPass>> m_BasePasses;
	std::vector<std::shared_ptr<RenderPass>> m_VPPasses;	//vp 상태일떄만 쓰는 패스
	std::vector<std::shared_ptr<RenderPass>> m_IndepentCulling;

	std::shared_ptr<DeferredPass> m_DeferredPass;
	std::shared_ptr<TransparencyPass> m_TransparencyPass;
	std::shared_ptr<DebugPass> m_DebugPass;
	std::shared_ptr<ObjectMaskPass> m_ObjectMaskPass;
	std::shared_ptr<ParticlePass> m_ParticlePass;
	std::shared_ptr<class UIPass> m_UIPass;
	std::shared_ptr<OutlineEdgeDetectPass> m_OutlineEdgeDetectPass;
	std::shared_ptr<OutlineBlurPass> m_OutlineBlurPass;
	std::shared_ptr<OutlineAddPass> m_OutlineAddPass;
	std::shared_ptr<GeoMetryPass> m_GeometryPass;
	std::shared_ptr<VPOutLinePass> m_VPOutLinePass;
	std::shared_ptr<RimLight> m_RimLight;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
	std::weak_ptr<DebugDrawManager> m_DebugDrawManager;
	std::shared_ptr<ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager = nullptr;
	std::shared_ptr<UIManager> m_UIManager;
	std::shared_ptr<LightManager> m_LightManager;

	VPMath::Matrix m_View;
	VPMath::Matrix m_Proj;

private:
	bool m_isVP = false;
};

