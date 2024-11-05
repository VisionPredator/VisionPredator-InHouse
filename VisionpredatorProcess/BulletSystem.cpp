#include "pch.h"
#include "BulletSystem.h"
#include "VisPredComponents.h"

BulletSystem::BulletSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
}

void BulletSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(ShotGunBulletComponent, bulletcomp)
	{
		float distance = (bulletcomp.GetComponent<TransformComponent>()->World_Location - bulletcomp.StartPoint).Length();
		if (distance> bulletcomp.Distance)
		{
		GetSceneManager()->DestroyEntity(bulletcomp.GetEntityID(),false);
		}
	}
}


void BulletSystem::EnterCollision(std::pair<uint32_t, uint32_t> entitypair)
{
	auto& Firstentity = *GetSceneManager()->GetEntity(entitypair.first);
	auto& Secondentity = *GetSceneManager()->GetEntity(entitypair.second);
	if (Firstentity.HasComponent<BulletComponent>())
	{
		ApplyDamage(Firstentity, Secondentity);
		return;
	}
	else if (Secondentity.HasComponent<BulletComponent>())
	{
		ApplyDamage(Secondentity, Firstentity);
		return;
	}

}

void BulletSystem::ApplyDamage(Entity& bullet, Entity& Other)
{
	if (Other.HasComponent<EnemyComponent>())
	{
		EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair<uint32_t, int >(Other.GetEntityID(), bullet.GetComponent<BulletComponent>()->Damage));

	}
	else if (Other.GetComponent<RigidBodyComponent>()->DefaultColliderInfo.PhysicsLayer == EPhysicsLayer::MAP)
	{
		auto transform = bullet.GetComponent<TransformComponent>();

		GetSceneManager()->SpawnEditablePrefab("../Data/Prefab/Decal(1).prefab", transform->World_Location, {0,0,0});
	}
	GetSceneManager()->DestroyEntity(bullet.GetEntityID());

}

void BulletSystem::ApplyShotGunDamage(std::shared_ptr<Entity> bullet, std::shared_ptr<Entity>  Other)
{
	if (Other->HasComponent<EnemyComponent>())
	{
		auto bullettrans = bullet->GetComponent<TransformComponent>();
		auto shotgunbullet = bullet->GetComponent<ShotGunBulletComponent>();

		shotgunbullet->Distance;
		float distance = (bullettrans->World_Location - shotgunbullet->StartPoint).Length();
		int applyDamage{};
		if (distance < shotgunbullet->Distance / 3)
			applyDamage = shotgunbullet->Damage1;
		else if (distance < shotgunbullet->Distance * 2 / 3)
			applyDamage = shotgunbullet->Damage2;

		else if (distance < shotgunbullet->Distance)
			applyDamage = shotgunbullet->Damage3;

		EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(Other->GetEntityID(), applyDamage));
	}
}

void BulletSystem::Initialize()
{
	COMPLOOP(BulletComponent, bulletcomp)
		Start(bulletcomp.GetEntityID());
	COMPLOOP(ShotGunBulletComponent, bulletcomp)
		Start(bulletcomp.GetEntityID());
}

void BulletSystem::Start(uint32_t gameObjectId)
{
	auto& entity = *GetSceneManager()->GetEntity(gameObjectId);
	if ( !entity.HasComponent<RigidBodyComponent>())
		return;
	if (entity.HasComponent<BulletComponent>() )
	{
		const auto& bulletcomp = *entity.GetComponent<BulletComponent>();;
		const auto& transform = *entity.GetComponent <TransformComponent >();
		m_PhysicsEngine->AddVelocity(gameObjectId, transform.FrontVector, bulletcomp.Speed);
	}
	else if	(entity.HasComponent<ShotGunBulletComponent>())
	{
		auto& bulletcomp = *entity.GetComponent<ShotGunBulletComponent>();;
		const auto& transform = *entity.GetComponent <TransformComponent >();
		m_PhysicsEngine->AddVelocity(gameObjectId, transform.FrontVector, bulletcomp.Speed);
		bulletcomp.StartPoint = transform.World_Location;
	}

}



void BulletSystem::EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{

	if (first->HasComponent<ShotGunBulletComponent>())
		ApplyShotGunDamage(first, second);
	else if (second->HasComponent<ShotGunBulletComponent>())
		ApplyShotGunDamage(second, first);
}

void BulletSystem::ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
}

