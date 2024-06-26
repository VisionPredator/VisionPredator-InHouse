#pragma once
#include "MeshFilter.h"
#include "RenderPass.h"

/// <summary>
/// �������� ������ ����� ����ϴ� ����������
///	������ ó���� �̸� �� �� ������Ʈ�� �׸���.
/// </summary>

class ModelData;

class ForwardPipeline
{
public:
	ForwardPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource);
	~ForwardPipeline();

	void Initialize();
	void Update(std::map<uint32_t, std::shared_ptr<RenderData>>& RenderList);

	void Render();

private:
	void CheckPassState(std::shared_ptr<RenderData>& model, PassState pass);


private:
	std::unordered_map<PassState,std::shared_ptr<RenderPass>> m_Passes;

	std::weak_ptr<Device> m_Device;
	std::weak_ptr<ResourceManager> m_ResourceManager;
};

