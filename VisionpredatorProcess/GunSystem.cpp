#include "pch.h"
#include "GunSystem.h"
#include "VisPredComponents.h"
#include"EventManager.h"
GunSystem::GunSystem(std::shared_ptr<SceneManager> scenemanager) :System{ scenemanager }
{
	EventManager::GetInstance().GetInstance().Subscribe("OnShoot", CreateSubscriber(&GunSystem::OnShoot));
	EventManager::GetInstance().GetInstance().Subscribe("OnInterected", CreateSubscriber(&GunSystem::OnInterected));
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
	auto gunID = std::any_cast<uint32_t>(entityID);
	const auto gunComp = GetSceneManager()->GetComponent<GunComponent>(gunID);
	auto particle = GetSceneManager()->GetChildEntityComp_HasComp<ParticleOwnerComponent>(gunID);
	if (particle)
	{
		EventManager::GetInstance().ImmediateEvent("OnFollowParticle", particle->GetEntityID());
	}
	//OnSpawnParticle
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
	//if (particle != nullptr)
	//{
	//	particle->IsRender = true;
	//	particle->Restart = true;
	//}
}


void GunSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);
	if (interected->HasComponent<GunComponent>() && interector->HasComponent<PlayerComponent>())
	{
		auto& guncomp = *interected->GetComponent<GunComponent>();
		auto& player = *interector->GetComponent<PlayerComponent>();
		Interected_Gun(guncomp, player);
	}
}
/// <summary>
/// GrapGun
/// </summary>
/// <param name="guncomp"></param>
/// <param name="playercomp"></param>
void GunSystem::Interected_Gun(GunComponent& guncomp, PlayerComponent& playercomp)
{
	auto anicomp = playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	auto soundcomp = playercomp.GetComponent<PlayerSoundComponent>();
	if (anicomp->IsBlending)
		return;
	if (anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Sword
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Pistol
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Rifle
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_ShotGun
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToThrow_Pistol
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToThrow_Rifle
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToThrow_ShotGun
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToAttack1_Sword
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToAttack2_Sword
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToAttack3_Sword
		)
		return;

	if ((anicomp->PlayerCurAni == VisPred::Game::PlayerAni::ToThrow_Pistol
		|| anicomp->PlayerCurAni == VisPred::Game::PlayerAni::ToThrow_Rifle
		|| anicomp->PlayerCurAni == VisPred::Game::PlayerAni::ToThrow_ShotGun) && !anicomp->IsFinished)
	{
		return;
	}

	if (playercomp.HasGun)
	{
		std::any data = std::ref(playercomp);
		EventManager::GetInstance().ImmediateEvent("OnDrop_Gun", data);
	}

	uint32_t handID = playercomp.HandEntity.lock()->GetEntityID();

	if (guncomp.HasComponent<Parent>())
	{
		GetSceneManager()->RemoveParent(guncomp.GetEntityID());
	}

	auto soceketcomp = guncomp.GetComponent<SocketComponent>();
	soceketcomp->IsConnected = true;
	soceketcomp->ConnectedEntityID = handID;
	playercomp.HasGun = true;
	playercomp.GunEntityID = guncomp.GetEntityID();
	m_PhysicsEngine->ConvertToStaticWithLayer(playercomp.GunEntityID, VPPhysics::EPhysicsLayer::NONCOLLISION);
	guncomp.GetComponent<MeshComponent>()->IsOverDraw = true;
	///TODO 사운드 로직 추가하기.
	playercomp.LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = false;

	VisPred::Engine::AniBlendData temp{ playercomp.HandEntity.lock()->GetEntityID(), {}, 0 ,0, false };

	switch (guncomp.Type)
	{
	case VisPred::Game::GunType::PISTOL:
		temp.Index = static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_Pistol);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		temp.Index = static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_ShotGun);
		break;
	case VisPred::Game::GunType::RIFLE:
		temp.Index = static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_Rifle);
		break;
	default:
		break;
	}
	std::any data = temp;
	EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	GetSceneManager()->SpawnSoundEntity(soundcomp->SoundKey_GunDraw, soundcomp->Volume_GunDraw, true, false, {});
	EventManager::GetInstance().ImmediateEvent("OnUpdateWeaponUI");
			
}

