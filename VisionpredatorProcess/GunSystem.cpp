#include "pch.h"
#include "GunSystem.h"
#include "VisPredComponents.h"

GunSystem::GunSystem(std::shared_ptr<SceneManager> scenemanager) :System{ scenemanager }
{
}

void GunSystem::Update(float deltaTime)
{
	COMPLOOP(GunComponent, guncomp)
	{
		if (guncomp.CurrentBullet==0)
			guncomp.IsEmpty = true;
	}
}

void GunSystem::EnterCollision(std::pair<uint32_t, uint32_t> entitypair)
{
	if (GetSceneManager()->HasComponent<GunComponent>(entitypair.first))
	{
		if (GetSceneManager()->HasComponent<ControllerComponent>(entitypair.second))
		{
			if (GetSceneManager()->GetEntity(entitypair.second)->HasComponent<EnemyComponent>())
			{
				auto& guncomp = *GetSceneManager()->GetComponent<GunComponent>(entitypair.first);
				if (guncomp.IsEmpty&& guncomp.GetComponent<RigidBodyComponent>()->Speed.Length() > 1.f)
				{
					GetSceneManager()->GetEntity(entitypair.second)->GetComponent<EnemyComponent>()->HP -= guncomp.ThrowDamage;
					GetSceneManager()->GetEntity(entitypair.second)->GetComponent<EnemyComponent>()->OnHit = true;
				}
			}

			//GetSceneManager()->DestroyEntity(entitypair.first);
		}
		//else
			//GetSceneManager()->DestroyEntity(entitypair.first);
	}
	else if (GetSceneManager()->HasComponent<GunComponent>(entitypair.second))
	{
		if (GetSceneManager()->HasComponent<ControllerComponent>(entitypair.first))
		{
			if (GetSceneManager()->GetEntity(entitypair.first)->HasComponent<EnemyComponent>())
			{
				auto& guncomp = *GetSceneManager()->GetComponent<GunComponent>(entitypair.second);
				if (guncomp.IsEmpty && guncomp.GetComponent<RigidBodyComponent>()->Speed.Length() > 1.f)
				{
					GetSceneManager()->GetEntity(entitypair.first)->GetComponent<EnemyComponent>()->HP -= guncomp.ThrowDamage;
					GetSceneManager()->GetEntity(entitypair.first)->GetComponent<EnemyComponent>()->OnHit = true;
				}
			}

			//GetSceneManager()->DestroyEntity(entitypair.second);
		}
		//else
			//GetSceneManager()->DestroyEntity(entitypair.second);
	}

}

void GunSystem::ExitCollision(std::pair<uint32_t, uint32_t> entitypair)
{
}
