#include "pch.h"
#include "GunSystem.h"
#include "VisPredComponents.h"
#include"EventManager.h"
GunSystem::GunSystem(std::shared_ptr<SceneManager> scenemanager) :System{ scenemanager }
{
	EventManager::GetInstance().GetInstance().Subscribe("OnShoot", CreateSubscriber(&GunSystem::OnShoot));
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


	if (Other.HasComponent<EnemyComponent>())
	{
		if (guncomp->IsEmpty && guncomp->GetComponent<RigidBodyComponent>()->Speed.Length() > 1.f)
		{
			EventManager::GetInstance().ImmediateEvent("OnDamaged", std::make_pair(Other.GetEntityID(), gun.GetComponent<GunComponent>()->ThrowDamage));
			GetSceneManager()->DestroyEntity(gun.GetEntityID());
		}

	}

}

void GunSystem::OnShoot(std::any entityID)
{
	const auto& gunID = std::any_cast<uint32_t>(entityID);
	const auto gunComp = GetSceneManager()->GetComponent<GunComponent>(gunID);
	const auto& particle = GetSceneManager()->GetChildEntityComp_HasComp<ParticleComponent>(gunID);
	// 오류로 인하여 주석처리.	
	// 총 발사 라이트
	//if (particle != nullptr)
	//{
	//	const auto transform = particle->GetComponent<TransformComponent>();
	//
	//	const auto& pointLightPrefabName = gunComp->MuzzleEffectPointLightPrefab;
	//	GetSceneManager()->SpawnEditablePrefab(pointLightPrefabName, transform->World_Location, transform->World_Rotation, transform->World_Scale);
	//}
	// Gun 오브젝트가 ParticleObj를 가지고 있는지 확인
	if (particle != nullptr)
	{
		particle->IsRender = true;
		particle->Restart = true;
	}
}

