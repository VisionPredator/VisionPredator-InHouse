#include "pch.h"
#include "BulletSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
BulletSystem::BulletSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnBulletHit", CreateSubscriber(&BulletSystem::OnBulletHit));
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

	if (&Firstentity == nullptr || &Secondentity == nullptr)
		return;

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
	// Enemy에 충돌했을 때
	if (Other.HasComponent<EnemyComponent>())
	{
		const auto bulletTransform = bullet.GetComponent<TransformComponent>();

		// 피격 이펙트 출력
		if (BloodSplashEntity.lock())
		{
			auto& particleTransform = BloodSplashEntity.lock()->GetComponent<TransformComponent>()->World_Location;
			const auto particleComp = BloodSplashEntity.lock()->GetComponent<ParticleComponent>();

			// 충돌 위치로 파티클 이동하여 렌더
			particleTransform = bulletTransform->World_Location;
			particleComp->IsRender = true;
			particleComp->Restart = true;
		}

		EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(Other.GetEntityID(), bullet.GetComponent<BulletComponent>()->Damage));
	}
	// 지형물에 충돌했을 때
	else if (Other.GetComponent<RigidBodyComponent>()->DefaultColliderInfo.PhysicsLayer == EPhysicsLayer::MAP)
	{
		auto bulletTransform = bullet.GetComponent<TransformComponent>();

		GetSceneManager()->SpawnEditablePrefab("../Data/Prefab/BulletMark.prefab", bulletTransform->World_Location, {0,0,0});

		// 바닥 총알 튀김 이펙트 출력
		if (GunSparkParticleEntity.lock())
		{
			auto& particleTransform = GunSparkParticleEntity.lock()->GetComponent<TransformComponent>()->World_Location;
			const auto particleComp = GunSparkParticleEntity.lock()->GetComponent<ParticleComponent>();

			// 충돌 위치로 파티클 이동하여 렌더
			particleTransform = bulletTransform->World_Location;
			particleComp->IsRender = true;
			particleComp->Restart = true;
		}
	}
	GetSceneManager()->DestroyEntity(bullet.GetEntityID());
}



void BulletSystem::ApplyShotGunDamage(std::shared_ptr<Entity> bullet, std::shared_ptr<Entity>  Other)
{
	// Enemy에 충돌했을 때
	if (Other->HasComponent<EnemyComponent>())
	{
		const auto bulletTransform = bullet->GetComponent<TransformComponent>();
		const auto bulletComp = bullet->GetComponent<ShotGunBulletComponent>();
		const float distance = (bulletTransform->World_Location - bulletComp->StartPoint).Length();
		int applyDamage{};

		if (distance < bulletComp->Distance / 3)
			applyDamage = bulletComp->Damage1;
		else if (distance < bulletComp->Distance * 2 / 3)
			applyDamage = bulletComp->Damage2;
		else if (distance < bulletComp->Distance)
			applyDamage = bulletComp->Damage3;

		// 피격 이펙트 출력
		GunsSparkOn(bulletTransform->World_Location);
		const auto enemypose = Other->GetComponent<TransformComponent>();
		VPMath::Vector3 bloodpose = enemypose->World_Location;
		bloodpose.y += 1.f;
		BloodSplashOn(bloodpose);
		EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(Other->GetEntityID(), applyDamage));
	}
	// 지형물에 맞았을 때
	else if (Other->GetComponent<RigidBodyComponent>()->DefaultColliderInfo.PhysicsLayer == EPhysicsLayer::MAP)
	{
		const auto bulletTransform = bullet->GetComponent<TransformComponent>();
		GunsSparkOn(bulletTransform->World_Location);
	}
}

void BulletSystem::Initialize()
{
	COMPLOOP(ParticleComponent, particlecomp)
		Start(particlecomp.GetEntityID());
	COMPLOOP(BulletComponent, bulletcomp)
		Start(bulletcomp.GetEntityID());
	COMPLOOP(ShotGunBulletComponent, bulletcomp)
		Start(bulletcomp.GetEntityID());
}

void BulletSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);

	if (entity->HasComponent<ShotGunBulletComponent>())
	{
		auto& bulletcomp = *entity->GetComponent<ShotGunBulletComponent>();;
		const auto& transform = *entity->GetComponent <TransformComponent >();
		m_PhysicsEngine->AddVelocity(gameObjectId, transform.FrontVector, bulletcomp.Speed);
		bulletcomp.StartPoint = transform.World_Location;
	}
	else if (entity->HasComponent<ParticleComponent>() && entity->HasComponent<IdentityComponent>())
	{
		if (entity->GetComponent<IdentityComponent>()->UUID == "GunSparkEffect")
			GunSparkParticleEntity = entity;
		else if (entity->GetComponent<IdentityComponent>()->UUID == "BloodSplashEffect")
			BloodSplashEntity = entity;
	}
	else if (entity->HasComponent<BulletComponent>())
	{
		auto& bulletcomp = *entity->GetComponent<BulletComponent>();;
		const auto& transform = *entity->GetComponent <TransformComponent >();
		m_PhysicsEngine->AddVelocity(gameObjectId, transform.FrontVector, bulletcomp.Speed);

	}
}

void BulletSystem::GunsSparkOn(VPMath::Vector3 pose)
{
	if (!GunSparkParticleEntity.lock())
		return;
	auto& particleTransform = GunSparkParticleEntity.lock()->GetComponent<TransformComponent>()->World_Location;
	const auto particleComp = GunSparkParticleEntity.lock()->GetComponent<ParticleComponent>();
	// 총알 위치로 파티클 이동
	particleTransform = pose;
	particleComp->IsRender = true;
	particleComp->Restart = true;
}

void BulletSystem::BloodSplashOn(VPMath::Vector3 pose)
{
	if (!BloodSplashEntity.lock())
		return;
	auto& particleTransform = BloodSplashEntity.lock()->GetComponent<TransformComponent>()->World_Location;
	const auto particleComp = BloodSplashEntity.lock()->GetComponent<ParticleComponent>();
	// 충돌 위치로 파티클 이동하여 렌더
	particleTransform = pose;
	particleComp->IsRender = true;
	particleComp->Restart = true;

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

void BulletSystem::OnBulletHit(std::any id_damage_pose)
{
	auto [entityid, damage, pose] = std::any_cast<std::tuple<uint32_t, int, VPMath::Vector3>>(id_damage_pose);

	auto entity = GetSceneManager()->GetEntity(entityid);

	if (!entity)
		return;
	if (entity->HasComponent<RigidBodyComponent>())
	{
		auto rigid = entity->GetComponent<RigidBodyComponent>();
		if (rigid->DefaultColliderInfo.PhysicsLayer == VPPhysics::EPhysicsLayer::MAP)
		{
			// 바닥 총알 튀김 이펙트 출력
			GunsSparkOn(pose);
			GetSceneManager()->SpawnEditablePrefab("../Data/Prefab/BulletMark.prefab", pose, { 0,0,0 });
			GetSceneManager()->SpawnSoundEntity("Gunhit", 10.f, false, false, pose);
		}
	}
	else if (entity->HasComponent<EnemyComponent>())
	{
		BloodSplashOn(pose);
		// 피격 이펙트 출력
		EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(entity->GetEntityID(), damage));
	}
}
