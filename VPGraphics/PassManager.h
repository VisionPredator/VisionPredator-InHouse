#pragma once
#include "MeshFilter.h"

#include "ForwardPass.h"
#include "DebugPass.h"
#include "DeferredPass.h"

/// <summary>
/// 전통적인 렌더링 기법을 사용하는 파이프라인
///	라이팅 처리를 미리 한 후 오브젝트를 그린다.
/// </summary>

class ModelData;
class DebugDrawManager;

class PassManager
{
public:
	PassManager(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource, std::shared_ptr<DebugDrawManager> debug);
	~PassManager();

	void Initialize();
	void Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList);

	void Render();

private:
	void CheckPassState(std::shared_ptr<RenderData>& model, PassState pass);
	void DrawGBuffer();
	void DrawIMGUI();

private:
	std::unordered_map<PassState,std::shared_ptr<RenderPass>> m_Passes;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
	std::weak_ptr<DebugDrawManager> m_DebugDrawManager;
};

