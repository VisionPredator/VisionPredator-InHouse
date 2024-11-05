#include "pch.h"
#include "IdentityRemoverSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"

IdentityRemoverSystem::IdentityRemoverSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&IdentityRemoverSystem::OnInterected));
}

void IdentityRemoverSystem::OnInterected(std::any entitys)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(entitys);
	if (!interected->HasComponent<IdentityRemoverComponent>())
		return;

	auto remover = interected->GetComponent<IdentityRemoverComponent>();
	if (remover->RemoveIdentitys.empty())
		return;
	for (auto& indentity : remover->RemoveIdentitys)
	{
		auto entity = GetSceneManager()->GetEntityByIdentityName(indentity);
		if (entity)
			GetSceneManager()->DestroyEntity(entity->GetEntityID());
	}

}
