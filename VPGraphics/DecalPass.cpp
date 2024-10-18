#include "pch.h"
#include "DecalPass.h"

DecalPass::DecalPass(const std::shared_ptr<Device>& device, const std::shared_ptr<ResourceManager>& resourceManager, const std::shared_ptr<DecalManager> decalmanager) : RenderPass(device,resourceManager)
{
	m_DecalManager = decalmanager;
}

DecalPass::~DecalPass()
{

}

void DecalPass::Render()
{
	//bind


	//render
	std::map<std::string, std::vector<decal::Info>>& curDecals = m_DecalManager->GetDecals();




	m_DecalManager->ClearDecals();
}

void DecalPass::OnResize()
{

}
