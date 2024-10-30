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

	auto& Firstentity = *GetSceneManager()->GetEntity(entitypair.first);
	auto& Secondentity = *GetSceneManager()->GetEntity(entitypair.second);
	if (Firstentity.HasComponent<GunComponent>())
		ApplyDamage(Firstentity, Secondentity);
	else if (Secondentity.HasComponent<GunComponent>())
		ApplyDamage(Secondentity, Firstentity);


}

void GunSystem::ExitCollision(std::pair<uint32_t, uint32_t> entitypair)
{
}


void GunSystem::ApplyDamage(Entity& gun, Entity& Other)
{
	auto guncomp = gun.GetComponent<GunComponent>();
	if (!guncomp->SoundEntity.lock())
	{
		guncomp->SoundEntity = GetSceneManager()->SpawnSoundEntity(guncomp->SoundKey_GunDrop, guncomp->Volume_GunDrop, false, false, guncomp->GetComponent<TransformComponent>()->World_Location);
	}
	else
	{
		int a = 5;
		a = 6;
	}

	if (Other.HasComponent<EnemyComponent>())
	{
		if (guncomp->IsEmpty && guncomp->GetComponent<RigidBodyComponent>()->Speed.Length() > 1.f)
		{
			EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair<uint32_t, int >(Other.GetEntityID(), gun.GetComponent<GunComponent>()->ThrowDamage));
			GetSceneManager()->DestroyEntity(gun.GetEntityID());
		}

	}

}