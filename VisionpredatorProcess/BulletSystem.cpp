#include "pch.h"
#include "BulletSystem.h"
#include "VisPredComponents.h"

BulletSystem::BulletSystem(std::shared_ptr<SceneManager> sceneManager): System(sceneManager)
{
}

void BulletSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(BulletComponent, comp)
	{
		auto transform = comp.GetComponent <TransformComponent >();
		VPMath::Vector3 addlocation{};
		addlocation = transform->FrontVector * comp.Speed * deltaTime;
		transform->AddWorldLocation(addlocation);
	}
}



void BulletSystem::EnterCollision(std::pair<uint32_t, uint32_t> entitypair)
{
	if (GetSceneManager()->HasComponent<BulletComponent>(entitypair.first))
	{
		if (GetSceneManager()->HasComponent<ControllerComponent>(entitypair.second))
		{
			if (GetSceneManager()->GetEntity(entitypair.second)->HasComponent<EnemyComponent>())
			{
				auto damage = GetSceneManager()->GetComponent<BulletComponent>(entitypair.first)->Damage;
				GetSceneManager()->GetEntity(entitypair.second)->GetComponent<EnemyComponent>()->HP -= damage;
				GetSceneManager()->GetEntity(entitypair.second)->GetComponent<EnemyComponent>()->OnHit = true;
			}

			GetSceneManager()->DestroyEntity(entitypair.first);
		}
		else
			GetSceneManager()->DestroyEntity(entitypair.first);
	}
	else if (GetSceneManager()->HasComponent<BulletComponent>(entitypair.second))
	{
		if (GetSceneManager()->HasComponent<ControllerComponent>(entitypair.first))
		{
			if (GetSceneManager()->GetEntity(entitypair.first)->HasComponent<EnemyComponent>())
			{
				auto damage = GetSceneManager()->GetComponent<BulletComponent>(entitypair.second)->Damage;
				GetSceneManager()->GetEntity(entitypair.first)->GetComponent<EnemyComponent>()->HP -= damage;
				GetSceneManager()->GetEntity(entitypair.first)->GetComponent<EnemyComponent>()->OnHit = true;
			}

			GetSceneManager()->DestroyEntity(entitypair.second);
		}
		else
			GetSceneManager()->DestroyEntity(entitypair.second);
	}
}

void BulletSystem::ExitCollision(std::pair<uint32_t, uint32_t> entitypair)
{
}
