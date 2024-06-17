#pragma once


#include "MeshFilter.h"
#include "RenderPass.h"



/// <summary>
/// 2024.06.05
/// 늘어나는 pass의 인스턴스를 관리하자 각종 render pass를 관리할 매니저 클래스
/// 그냥 패스를 타면 foward
/// 패스를 타기전에 gbuffer 뽑아서 무언가를하고 패스를 타면 deferred 쉽게 말하면 gbuffer + foward

///
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
	std::weak_ptr<ResourceManager> m_Resourcemanager;
};

