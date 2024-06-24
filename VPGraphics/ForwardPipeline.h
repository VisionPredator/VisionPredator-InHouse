#pragma once
#include "MeshFilter.h"
#include "RenderPass.h"

/// <summary>
/// 전통적인 렌더링 기법을 사용하는 파이프라인
///	라이팅 처리를 미리 한 후 오브젝트를 그린다.
/// </summary>

class ModelData;

class ForwardPipeline
{
public:
	ForwardPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource);
	~ForwardPipeline();

	void Initialize();
	void Update(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& RenderList);

	void Render();

private:
	std::unordered_map<PassState,std::shared_ptr<RenderPass>> m_Passes;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
};

