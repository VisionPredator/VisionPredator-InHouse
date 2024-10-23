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

void AreaAttackSystem::FixedUpdate(float deltaTime)
{

	COMPLOOP(AreaAttackComponent, areacomp)
	{
		if (!areacomp.AttachedEntity.lock())
			continue;

		auto& attachtrans = *areacomp.AttachedEntity.lock().get()->GetComponent<TransformComponent>();
		auto& maintrans = *areacomp.GetComponent<TransformComponent>();
		maintrans.SetLocalLocation(attachtrans.World_Location);
		maintrans.SetLocalQuaternion(attachtrans.World_Quaternion);
	}



}

void AreaAttackSystem::Initialize()
{
	COMPLOOP(AreaAttackComponent, areacomp)
		Start(areacomp.GetEntityID());
}

void AreaAttackSystem::Start(uint32_t gameObjectId)
{
	auto& entity = *GetSceneManager()->GetEntity(gameObjectId);
	if (!entity.HasComponent<AreaAttackComponent>())
		return;
	auto& area = *entity.GetComponent<AreaAttackComponent>();
	auto attachentity = GetSceneManager()->GetEntityByIdentityName(area.IdentityAttach);
	if (attachentity)
	{
		area.AttachedEntity = attachentity;
	}


}

void AreaAttackSystem::Finish(uint32_t gameObjectId)
{
}

void AreaAttackSystem::Finalize()
{
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


