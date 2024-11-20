#include "pch.h"
#include "AreaAttackSystem.h"

#include "StatesInclude.h"
#include "EnemyComponent.h"
#include "AreaAttackComponent.h"

AreaAttackSystem::AreaAttackSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
}

void AreaAttackSystem::TriggerAttack(AreaAttackComponent& area, EnemyComponent& enemy)
{
	EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(enemy.GetEntityID(), area.Damage));

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

void AreaAttackSystem::EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (first->HasComponent<AreaAttackComponent>()&& second->HasComponent<EnemyComponent>())
		TriggerAttack(*first->GetComponent<AreaAttackComponent>(), *second->GetComponent<EnemyComponent>());
	else if (first->HasComponent<EnemyComponent>() && second->HasComponent<AreaAttackComponent>())
		TriggerAttack(*second->GetComponent<AreaAttackComponent>(), *first->GetComponent<EnemyComponent>());

}


void AreaAttackSystem::ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{

}


