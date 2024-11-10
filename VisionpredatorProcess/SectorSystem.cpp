#include "pch.h"
#include "SectorSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
SectorSystem::SectorSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnEnemyKilled", CreateSubscriber(&SectorSystem::OnEnemyKilled));
}

void SectorSystem::OnEnemyKilled(std::any enemyid)
{
	auto entityid = std::any_cast<uint32_t>(enemyid);
	auto sectorcomp = GetSceneManager()->GetParentEntityComp_HasComp<SectorClearComponent>(entityid);
	if (!sectorcomp)
		return;
	//auto sectorcomp = sectorentity->GetComponent<SectorClearComponent>();
	CheckSectorClear(sectorcomp);
}

void SectorSystem::CheckSectorClear(SectorClearComponent* sectorcomp)
{
	if (!sectorcomp->HasComponent<Children>())
		return;

	auto childrens = sectorcomp->GetComponent<Children>()->ChildrenID;
	bool isSectorclear = true;
	for (auto childID : childrens)
	{
		auto entity = GetSceneManager()->GetEntity(childID);
		if (!entity)
			continue;
		if (!entity->HasComponent<EnemyComponent>())
			continue;
		EnemyComponent* enemycomp = entity->GetComponent<EnemyComponent>();

		if (enemycomp->BehaviorState != &EnemyBehaviorState::s_Dead)
		{
			isSectorclear = false;
			break;
		}
	}

	if (isSectorclear)
	{
		auto entity = GetSceneManager()->GetEntity(sectorcomp->GetEntityID());
		std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>> eventData(entity, nullptr);
		EventManager::GetInstance().ImmediateEvent("OnInterected", eventData);

	}

}
void SectorSystem::Initialize()
{
	COMPLOOP(SectorClearComponent, comp)
	{
		Start(comp.GetEntityID());

	}
}
void SectorSystem::Start(uint32_t gameObjectId)
{
}
void SectorSystem::Finish(uint32_t gameObjectId)
{
}
void SectorSystem::Finalize()
{
}
