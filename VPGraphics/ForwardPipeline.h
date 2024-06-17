#pragma once


#include "MeshFilter.h"
#include "RenderPass.h"



/// <summary>
/// 2024.06.05
/// �þ�� pass�� �ν��Ͻ��� �������� ���� render pass�� ������ �Ŵ��� Ŭ����
/// �׳� �н��� Ÿ�� foward
/// �н��� Ÿ������ gbuffer �̾Ƽ� ���𰡸��ϰ� �н��� Ÿ�� deferred ���� ���ϸ� gbuffer + foward

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

