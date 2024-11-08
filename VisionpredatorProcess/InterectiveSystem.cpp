#include "pch.h"
#include "InterectiveSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
#include <EngineStructs.h>
InterectiveSystem::InterectiveSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnSearched", CreateSubscriber(&InterectiveSystem::OnSearched));
	EventManager::GetInstance().Subscribe("OnUnSearched", CreateSubscriber(&InterectiveSystem::OnUnSearched));
	EventManager::GetInstance().Subscribe("OnInterective", CreateSubscriber(&InterectiveSystem::OnInterective));
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&InterectiveSystem::OnInterected));
}

void InterectiveSystem::OnSearched(std::any searchedEntityID)
{
	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;
	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();
	if (!interective->IsInterective)
		return;
	EventManager::GetInstance().ImmediateEvent("OnUpdateSearchUI");

	auto meshcompvec = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(interectedEntity->GetEntityID());
	if (meshcompvec.empty())
		return;
	for (auto meshcomp : meshcompvec)
		meshcomp->MaskColor = interective->SearcehdColor;

}

void InterectiveSystem::OnUnSearched(std::any searchedEntityID)
{

	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;

	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();
	EventManager::GetInstance().ImmediateEvent("OnResetInterectionUI");

	//if (!interective->IsInterective)
	//	return;
	auto meshcompvec = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(interectedEntity->GetEntityID());
	if (meshcompvec.empty())
		return;
	for (auto meshcomp : meshcompvec)
		meshcomp->MaskColor = {};
	//if (interectedEntity->HasComponent<MeshComponent>())
	//{
	//	auto meshcomp = interectedEntity->GetComponent<MeshComponent>();
	//	meshcomp->MaskColor = {};
	//}

}

void InterectiveSystem::OnInterective(std::any interective_interector)
{
	auto [first, second] = std::any_cast<std::pair<uint32_t, uint32_t>>(interective_interector);
	auto firstentity = GetSceneManager()->GetEntity(first);
	auto secondentity = GetSceneManager()->GetEntity(second);
	if (!firstentity || !secondentity)
		return;
	if (firstentity->HasComponent<InterectiveComponent>())
	{
		if (!firstentity->GetComponent<InterectiveComponent>()->IsInterective)
			return;
		std::pair< std::shared_ptr<Entity>, std::shared_ptr<Entity > > temp = { firstentity,secondentity };
		std::any data = temp;
		EventManager::GetInstance().ImmediateEvent("OnInterected", data);
	}
	else if (secondentity->HasComponent<InterectiveComponent>())
	{
		if (!secondentity->GetComponent<InterectiveComponent>()->IsInterective)
			return;
		std::pair< std::shared_ptr<Entity>, std::shared_ptr<Entity > > temp = { secondentity, firstentity };
		std::any data = temp;
		EventManager::GetInstance().ImmediateEvent("OnInterected", data);
	}


}

void InterectiveSystem::OnInterected(std::any interective_interector)
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
void InterectiveSystem::Interected_Gun(GunComponent& guncomp, PlayerComponent& playercomp)
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
		&& anicomp->PlayerCurAni != VisPred::Game::PlayerAni::ToThrow_ShotGun)
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

}

