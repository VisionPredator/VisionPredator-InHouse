#include "pch.h"
#include "SectorSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
SectorSystem::SectorSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnUpdateSector", CreateSubscriber(&SectorSystem::OnUpdateSector));
}

void SectorSystem::OnUpdateSector(std::any enemyid)
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

		if (sectorcomp->DoorIdentity.empty())
			return;
		auto doorentity = GetSceneManager()->GetEntityByIdentityName(sectorcomp->DoorIdentity);
		if (!doorentity->HasComponent<DoorComponent>())
			return;

		EventManager::GetInstance().ImmediateEvent("OnChangeDoorUseable", std::make_pair<uint32_t, bool>(doorentity->GetEntityID(), true));
	}

}
