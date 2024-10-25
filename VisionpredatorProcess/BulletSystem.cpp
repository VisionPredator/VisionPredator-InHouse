#include "pch.h"
#include "BulletSystem.h"
#include "VisPredComponents.h"

BulletSystem::BulletSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{
}


void BulletSystem::EnterCollision(std::pair<uint32_t, uint32_t> entitypair)
{
	auto& Firstentity = *GetSceneManager()->GetEntity(entitypair.first);
	auto& Secondentity = *GetSceneManager()->GetEntity(entitypair.second);
	if (Firstentity.HasComponent<BulletComponent>())
		ApplyDamage(Firstentity, Secondentity);
	else if (Secondentity.HasComponent<BulletComponent>())
		ApplyDamage(Secondentity, Firstentity);
}

void BulletSystem::ApplyDamage(Entity& bullet, Entity& Other)
{
	if (Other.HasComponent<EnemyComponent>())
	{
		auto damage = bullet.GetComponent<BulletComponent>()->Damage;
		Other.GetComponent<EnemyComponent>()->HP -= damage;
		Other.GetComponent<EnemyComponent>()->OnHit = true;
	}
	GetSceneManager()->DestroyEntity(bullet.GetEntityID());

}

void BulletSystem::Initialize()
{
	COMPLOOP(BulletComponent, bulletcomp)
		Start(bulletcomp.GetEntityID());
}

void BulletSystem::Start(uint32_t gameObjectId)
{
	auto& entity = *GetSceneManager()->GetEntity(gameObjectId);
	if (!entity.HasComponent<BulletComponent>() || !entity.HasComponent<RigidBodyComponent>())
		return;
	const auto& bulletcomp = *entity.GetComponent<BulletComponent>();;
	const auto& transform = *entity.GetComponent <TransformComponent >();
	m_PhysicsEngine->AddVelocity(gameObjectId, transform.FrontVector, bulletcomp.Speed);
}

