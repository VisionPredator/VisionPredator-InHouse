#pragma once
#include "MeshFilter.h"

#include "ForwardPass.h"
#include "DebugPass.h"
#include "DeferredPass.h"

/// <summary>
///	��� �н����� �����ϴ� �Ŵ���.
/// ���۵� ������ ����� ����Ѵ�.
/// </summary>

class ParticlePass;
class TimeManager;
class ParticleManager;
class ModelData;
class DebugDrawManager;

class PassManager
{
public:
	PassManager(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource, std::shared_ptr<DebugDrawManager> debug,
		const std::shared_ptr<ParticleManager>& particleManager);
	~PassManager();

	void Initialize();
	void Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList);

	void Render();
	void OnResize();

private:
	void CheckPassState(std::shared_ptr<RenderData>& model, PassState pass);
	void DrawGBuffer();
	void DrawIMGUI();

private:
	std::unordered_map<PassState,std::shared_ptr<RenderPass>> m_Passes;

	// �ӽ�
	std::shared_ptr<ParticlePass> m_ParticlePass;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
	std::weak_ptr<DebugDrawManager> m_DebugDrawManager;

	std::shared_ptr<ParticleManager> m_ParticleManager;
	TimeManager* m_TimeManager = nullptr;
	DirectX::SimpleMath::Matrix m_View;
	DirectX::SimpleMath::Matrix m_Proj;
};

