#include "pch.h"
#include "DoOnceSystem.h"
#include "DoOnceComponent.h"
#include "DoOnceResetComponent.h"
#include "InterectiveComponent.h"
#include"EventManager.h"
DoOnceSystem::DoOnceSystem(std::shared_ptr<SceneManager> scenemanger):System(scenemanger)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&DoOnceSystem::OnInterected));
}

void DoOnceSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);

	if (interected->HasComponent<DoOnceComponent>())
		DoOnce(interected->GetComponent<DoOnceComponent>());

	if (interected->HasComponent<DoOnceResetComponent>())
		DoOnceReset(interected->GetComponent<DoOnceResetComponent>());

}

void DoOnceSystem::DoOnceReset(DoOnceResetComponent* comp)
{
	comp->ResetIdentitys;
	for (auto& identity : comp->ResetIdentitys)
	{
		auto entity = GetSceneManager()->GetEntityByIdentityName(identity);
		if (entity && entity->HasComponent<InterectiveComponent>())
			entity->GetComponent<InterectiveComponent>()->IsInterective = true;
	}
}

void DoOnceSystem::DoOnce(DoOnceComponent* comp)
{

	comp->GetComponent<InterectiveComponent>()->IsInterective = false;
	EventManager::GetInstance().ImmediateEvent("OnUnSearched", comp->GetEntityID());

}


