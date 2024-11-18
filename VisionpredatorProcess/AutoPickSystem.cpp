#include "pch.h"
#include "AutoPickSystem.h"
#include "EventSubscriber.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include "AutoPickComponent.h"
#include "GunComponent.h"
#include "PlayerComponent.h"
AutoPickSystem::AutoPickSystem(std::shared_ptr<SceneManager> scenemnager) :System(scenemnager)
{
	EventManager::GetInstance().Subscribe("OnAutoPickup", CreateSubscriber(&AutoPickSystem::OnAutoPickup));
}

void AutoPickSystem::EnterTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (first->HasComponent<AutoPickComponent>() && second->HasComponent<GunComponent>())
		AddPickup(first->GetComponent<AutoPickComponent>(), second->GetComponent<GunComponent>());
	else if (first->HasComponent<GunComponent>() && second->HasComponent<AutoPickComponent>())
		AddPickup(second->GetComponent<AutoPickComponent>(), first->GetComponent<GunComponent>());

}

void AutoPickSystem::ExitTrigger(std::shared_ptr<Entity> first, std::shared_ptr<Entity> second)
{
	if (first->HasComponent<AutoPickComponent>() && second->HasComponent<GunComponent>())
		ReleasePickup(first->GetComponent<AutoPickComponent>(), second->GetComponent<GunComponent>());
	else if (first->HasComponent<GunComponent>() && second->HasComponent<AutoPickComponent>())
		ReleasePickup(second->GetComponent<AutoPickComponent>(), first->GetComponent<GunComponent>());
}

void AutoPickSystem::AddPickup(AutoPickComponent* autopick, GunComponent* pickuped)
{
	if (pickuped->IsEmpty)
		return;

	// ID�� �̹� PickUps ����Ʈ�� �ִ��� Ȯ��
	uint32_t entityID = pickuped->GetEntityID();
	if (std::find(autopick->PickUps.begin(), autopick->PickUps.end(), entityID) == autopick->PickUps.end())
	{
		autopick->PickUps.push_back(entityID);
	}
}

void AutoPickSystem::ReleasePickup(AutoPickComponent* autopick, GunComponent* pickuped)
{
	auto it = std::find(autopick->PickUps.begin(), autopick->PickUps.end(), pickuped->GetEntityID());
	if (it != autopick->PickUps.end())
	{
		autopick->PickUps.erase(it);
	}
}

void AutoPickSystem::OnAutoPickup(std::any pcomp)
{
	AutoPickComponent* autopick = std::any_cast<AutoPickComponent*>(pcomp);
	AutoPickUp(autopick);
}



void AutoPickSystem::FixedUpdate(float deltaTime)
{

	COMPLOOP(AutoPickComponent, pickupcomp)
	{
		if (pickupcomp.ParentPlayer
			&& pickupcomp.IsAuto
			&& !pickupcomp.ParentPlayer->HasGun
			&& !pickupcomp.ParentPlayer->IsVPMode
			&& (pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni == static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Sword)
				|| pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni == static_cast<int>(VisPred::Game::PlayerAni::ToIdle01_Sword)
				|| pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni == static_cast<int>(VisPred::Game::PlayerAni::ToAttack1_Sword)
				|| pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni == static_cast<int>(VisPred::Game::PlayerAni::ToAttack2_Sword)
				|| pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->curAni == static_cast<int>(VisPred::Game::PlayerAni::ToAttack3_Sword)	)
			&& !pickupcomp.ParentPlayer->HandEntity.lock()->GetComponent<AnimationComponent>()->IsBlending)
		{

			pickupcomp.ParentPlayer->IsAttacking = false;

			AutoPickUp(&pickupcomp);
		}
	}
}


void AutoPickSystem::Update(float deltaTime)
{
	COMPLOOP(AutoPickComponent, pickupcomp)
	{
		if (INPUTKEYDOWN(KEYBOARDKEY::F1))
		{
			pickupcomp.IsAuto = !pickupcomp.IsAuto;
		}
	}
}





void AutoPickSystem::Initialize()
{
	COMPLOOP(AutoPickComponent, autopick)
	{
		Start(autopick.GetEntityID());
	}
}

void AutoPickSystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<AutoPickComponent>(gameObjectId))
		return;
	auto pickup = GetSceneManager()->GetComponent<AutoPickComponent>(gameObjectId);

	pickup->ParentPlayer = GetSceneManager()->GetParentEntityComp_HasComp<PlayerComponent>(gameObjectId);

}

void AutoPickSystem::Finish(uint32_t gameObjectId)
{
}

void AutoPickSystem::Finalize()
{
}

void AutoPickSystem::AutoPickUp(AutoPickComponent* autopick)
{
	if (autopick->PickUps.empty())
	{
		autopick->ParentPlayer->LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = true;
		return;
	}

	// PickUps ����Ʈ���� �������� �ʴ� ��ƼƼ�� ����
	RemoveInvalidEntities(autopick);

	// �Ÿ� ��� �� ���� ����� ��ȿ�� ��ƼƼ ã��
	auto closestEntity = FindClosestValidEntity(autopick);

	if (closestEntity != 0)
		EventManager::GetInstance().ImmediateEvent("OnInterective", std::make_pair(autopick->ParentPlayer->GetEntityID(), closestEntity));
	else
		autopick->ParentPlayer->LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = true;
}




void AutoPickSystem::RemoveInvalidEntities(AutoPickComponent* autopick)
{
	for (auto it = autopick->PickUps.begin(); it != autopick->PickUps.end(); )
	{
		auto entity = GetSceneManager()->GetEntity(*it);
		if (!entity)
		{
			it = autopick->PickUps.erase(it);
		}
		else if (entity->HasComponent<GunComponent>() && entity->GetComponent<GunComponent>()->IsEmpty)
		{
			it = autopick->PickUps.erase(it);
		}
		else
		{
			++it;
		}
	}
}
bool AutoPickSystem::IsValidPickupEntity(uint32_t entityID)
{
	auto entity = GetSceneManager()->GetEntity(entityID);
	if (!entity || !m_PhysicsEngine->IsDynamic(entityID))
		return false;

	auto guncomp = entity->GetComponent<GunComponent>();
	if ((guncomp && !guncomp->IsEmpty))
		return true;
	return false;
}

uint32_t AutoPickSystem::FindClosestValidEntity(AutoPickComponent* autopick) {
	auto playerLocation = autopick->ParentPlayer->GetComponent<TransformComponent>()->World_Location;
	std::vector<std::pair<float, uint32_t>> distanceEntities;

	// �Ÿ� ��� �� ���Ϳ� �߰�
	for (auto gunid : autopick->PickUps)
	{
		auto gunEntity = GetSceneManager()->GetEntity(gunid);
		if (gunEntity) {
			float distance = (playerLocation - gunEntity->GetComponent<TransformComponent>()->World_Location).Length();
			distanceEntities.emplace_back(distance, gunid);
		}
	}
	// �Ÿ������� ����
	std::sort(distanceEntities.begin(), distanceEntities.end());

	// ��ȿ�� ���� ����� ��ƼƼ ã��
	for (const auto& [distance, entityID] : distanceEntities) {
		if (IsValidPickupEntity(entityID))
		{
			return entityID;
		}
	}
	return 0; // ��ȿ�� ��ƼƼ�� ������ 0�� ��ȯ
}