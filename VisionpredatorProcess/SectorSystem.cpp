#include "pch.h"
#include "SectorSystem.h"
#include "EventManager.h"
#include "SectorClearComponent.h"
#include "EnemyComponent.h"
#include "TextBounceComponent.h"
SectorSystem::SectorSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnEnemyKilled", CreateSubscriber(&SectorSystem::OnEnemyKilled));
	EventManager::GetInstance().Subscribe("OnHideEnemyCount", CreateSubscriber(&SectorSystem::OnHideEnemyCount));
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

	bool isSectorclear = true;
	auto enemys =GetSceneManager()->GetChildEntityComps_HasComp<EnemyComponent>(sectorcomp->GetEntityID());
	if (enemys.empty())
		isSectorclear = true;
	int LiveEnemy = 0;
	for (auto enemy : enemys)
	{
		if (enemy->BehaviorState != &EnemyBehaviorState::s_Dead)
		{
			isSectorclear = false;
			LiveEnemy++;
		}
	}

	if (!isSectorclear && sectorcomp->HasComponent<TextComponent>())
	{
		auto text = sectorcomp->GetComponent<TextComponent>();
		// Set the message with the enemy count
		text->Text = sectorcomp->Ment + std::to_wstring(LiveEnemy);
		if (sectorcomp->HasComponent<TextBounceComponent>())
		{
			sectorcomp->GetComponent<TextBounceComponent>()->AddedBounce = true;
		}
	}

	if (isSectorclear)
	{
		auto entity = GetSceneManager()->GetEntity(sectorcomp->GetEntityID());
		std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>> eventData(entity, nullptr);
		EventManager::GetInstance().ImmediateEvent("OnInterected", eventData);
		EventManager::GetInstance().ImmediateEvent("OnChangeTopic", VisPred::Game::TopicType::FINDBELL);
		if (sectorcomp->HasComponent<TextComponent>())
		{
			auto text = sectorcomp->GetComponent<TextComponent>();
			text->Text = {};
		}

	}

}
void SectorSystem::OnHideEnemyCount(std::any null)
{
	COMPLOOP(SectorClearComponent, sectorcomp)
	{

		if (sectorcomp.HasComponent<TextComponent>())
		{
			auto text = sectorcomp.GetComponent<TextComponent>();
			// Set the message with the enemy count
			text->Text = {};

		}

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
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity || !entity->HasComponent<SectorClearComponent>())
		return;
	CheckSectorClear(entity->GetComponent<SectorClearComponent>());
	EventManager::GetInstance().ImmediateEvent("OnChangeTopic", VisPred::Game::TopicType::KILLALL);
}
void SectorSystem::Finish(uint32_t gameObjectId)
{
}
void SectorSystem::Finalize()
{
}
