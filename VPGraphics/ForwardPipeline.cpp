#include "pch.h"
#include "ForwardPipeline.h"

#include "ModelData.h"



ForwardPipeline::ForwardPipeline(std::shared_ptr<Device> device, std::shared_ptr<ResourceManager> resource) : m_Device(device),m_Resourcemanager(resource)
{

}

ForwardPipeline::~ForwardPipeline()
{
	for (auto& pass : m_Passes)
	{
		pass.second.reset();
	}
	
	m_Passes.clear();
}

void ForwardPipeline::Initialize()
{
	m_Passes.insert(std::make_pair<PassState,std::shared_ptr<RenderPass>>(PassState::Static,std::make_shared<StaticPass>(m_Device.lock(),m_Resourcemanager.lock())));
	m_Passes.insert(std::make_pair<PassState,std::shared_ptr<RenderPass>>(PassState::Skinning,std::make_shared<SkinningPass>(m_Device.lock(), m_Resourcemanager.lock())));
	m_Passes.insert(std::make_pair<PassState,std::shared_ptr<RenderPass>>(PassState::Debug,std::make_shared<DebugPass>(m_Device.lock(), m_Resourcemanager.lock())));
}

void ForwardPipeline::Update(std::map<std::wstring, std::pair<PassState, std::shared_ptr<ModelData>>>& RenderList)
{
	int index = 0;

	//��Ʈ �������� �غ���
	for (auto& model : RenderList)
	{
		PassState curState = model.second.first;
		PassState temp = curState;
		 
		temp &= PassState::Static;
		if (temp == PassState::Static)
		{
			m_Passes[temp]->AddModelData(model.second.second);
		}

		temp = curState;
		temp &= PassState::Skinning;
		if (temp == PassState::Skinning)
		{
			m_Passes[temp]->AddModelData(model.second.second);
		}

		temp = curState;
		temp &= PassState::Debug;
		if (temp == PassState::Debug)
		{
			m_Passes[temp]->AddModelData(model.second.second);

		}

	}
}

void ForwardPipeline::Render()
{
	for (auto& pass : m_Passes)
	{
		pass.second->Render();
	}


}
