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
			GetSceneManager()->DestroyEntity(entitypair.second);
			GetSceneManager()->DestroyEntity(entitypair.first);
		}
		else
			GetSceneManager()->DestroyEntity(entitypair.first);
	}
	else if (GetSceneManager()->HasComponent<BulletComponent>(entitypair.second))
	{
		if (GetSceneManager()->HasComponent<ControllerComponent>(entitypair.first))
		{
			GetSceneManager()->DestroyEntity(entitypair.second);
			GetSceneManager()->DestroyEntity(entitypair.first);
		}
		else
			GetSceneManager()->DestroyEntity(entitypair.second);
	}
}

void BulletSystem::ExitCollision(std::pair<uint32_t, uint32_t> entitypair)
{
}
