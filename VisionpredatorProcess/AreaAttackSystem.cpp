#include "pch.h"
#include "AreaAttackSystem.h"

AreaAttackSystem::AreaAttackSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
}

void AreaAttackSystem::TriggerAttack(AreaAttackComponent& area, EnemyComponent& enemy)
{
	enemy.HP -= area.Damage;
	enemy.OnHit = true;
}

void AreaAttackSystem::EnterTrigger(std::pair<uint32_t, uint32_t> entitypair)
{
	auto entity_first = GetSceneManager()->GetEntity(entitypair.first);
	auto entity_second = GetSceneManager()->GetEntity(entitypair.second);

	if (entity_first->HasComponent<AreaAttackComponent>()&& entity_second->HasComponent<EnemyComponent>())
		TriggerAttack(*entity_first->GetComponent<AreaAttackComponent>(), *entity_second->GetComponent<EnemyComponent>());
	else if (entity_first->HasComponent<EnemyComponent>() && entity_second->HasComponent<AreaAttackComponent>())
		TriggerAttack(*entity_second->GetComponent<AreaAttackComponent>(), *entity_first->GetComponent<EnemyComponent>());

}


void AreaAttackSystem::ExitTrigger(std::pair<uint32_t, uint32_t> entitypair)
{

}


