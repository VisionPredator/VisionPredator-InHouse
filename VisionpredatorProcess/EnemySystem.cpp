#include "pch.h"
#include "EnemySystem.h"

#include <memory>

#include "VisPredComponents.h"
#include "EngineStructs.h"
#include "../VPGraphics/D3DUtill.h"

#include "EnemyMovementState.h"
#include "EnemyJumpState.h"
#include "StatesInclude.h"
#include "EnemyBehaviorState.h"

EnemySystem::EnemySystem(const std::shared_ptr<SceneManager>& sceneManager): System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnDamaged", CreateSubscriber(&EnemySystem::OnDamaged));
}

void EnemySystem::Initialize()
{
	PlayerComponent* playerComponent = nullptr;

	COMPLOOP(PlayerComponent, comp)
	{
		uint32_t playerID = comp.GetEntityID();
		if (GetSceneManager()->HasComponent<PlayerComponent>(playerID))
		{
			m_playercomponent = GetSceneManager()->GetComponent<PlayerComponent>(playerID);
			break;
		}
	}

	//for (auto& iter : m_SceneManager.lock()->GetComponentPool<IdentityComponent>())
	//{
	//	iter.get().UUID;
	//}

	COMPLOOP(EnemyComponent, comp)
	{
		Start(comp.GetEntityID());
	}
}

// 로우 포인터로 받은 객체를 스마트 포인터로 변환하였을 때 메모리 해제 동작을 하지 않도록 설정하기 위한 클래스
struct null_deleter
{
	void operator()(void const *) const {}
};

void EnemySystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->HasComponent<EnemyComponent>(gameObjectId))
		return;

	const auto enemyCompRawPtr = GetSceneManager()->GetComponent<EnemyComponent>(gameObjectId);
	const std::shared_ptr<EnemyComponent> enemyComp(enemyCompRawPtr, null_deleter{});	// null_deleter를 사용해 메모리 해제가 되지 않도록 스마트 포인터 생성

	enemyCompRawPtr->Player = m_playercomponent;
	enemyCompRawPtr->SceneManager = m_SceneManager;
	enemyCompRawPtr->PhysicsManager = m_PhysicsEngine;
	enemyCompRawPtr->Graphics = m_Graphics;
	enemyCompRawPtr->SoundEngine = m_SoundEngine;

	if (enemyComp->Player->CurrentFSM == PlayerFSM::DIE || enemyComp->Player->CurrentFSM == PlayerFSM::DIE_END)
		return;

	enemyComp->BehaviorState->Enter(enemyComp);
	enemyComp->CombatState->Enter(enemyComp);
	enemyComp->MovementState->Enter(enemyComp);
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, enemycomp)
	{
		if (enemycomp.Player->CurrentFSM == PlayerFSM::DIE || enemycomp.Player->CurrentFSM == PlayerFSM::DIE_END)
			return;

		const std::shared_ptr<EnemyComponent> enemyComp(&enemycomp, null_deleter{});	// null_deleter를 사용해 메모리 해제가 되지 않도록 스마트 포인터 생성

		enemycomp.BehaviorState->Update(enemyComp, deltaTime);
		enemycomp.CombatState->Update(enemyComp, deltaTime);
		enemycomp.MovementState->Update(enemyComp, deltaTime);

		//Log::GetClientLogger()->info("Current Behavior State: {}", std::string(typeid(*enemycomp.BehaviorState).name()).substr(6));
		//Log::GetClientLogger()->info("Current Movement State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));
	}
}

void EnemySystem::OnDamaged(std::any entityid_Damage)
{
	auto [entityid, damage] = std::any_cast<std::pair <uint32_t, int>>(entityid_Damage);

	auto entity = GetSceneManager()->GetEntity(entityid);
	if (!entity->HasComponent<EnemyComponent>())
		return;
	auto enemycomponent = entity->GetComponent<EnemyComponent>();
	if (enemycomponent->HP == 0)
		return;
	if (enemycomponent->HP > damage)
	{
		enemycomponent->OnHit = true;
		enemycomponent->HP -= damage;

		if (enemycomponent->MovementState == &EnemyMovementState::s_HitReaction)
		{
			const std::shared_ptr<EnemyComponent> enemyComp(enemycomponent, null_deleter{});	// null_deleter를 사용해 메모리 해제가 되지 않도록 스마트 포인터 생성
			enemycomponent->MovementState->Enter(enemyComp);
		}
		else
			enemycomponent->MovementState->ChangeCurrentState(*enemycomponent, &EnemyMovementState::s_HitReaction);
	}
	else
	{
		Log::GetClientLogger()->warn("HEAT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

		enemycomponent->HP = 0;

		int healthup{};
		if (m_playercomponent->IsVPMode)
			healthup = 20;
		else
			healthup = 10;
		if (m_playercomponent->CurrentFSM == VisPred::Game::PlayerFSM::DIE)
			return;
		if (m_playercomponent->HP<=0)
			return;
		if ((m_playercomponent->HP + healthup) > m_playercomponent->MaxHP)
			m_playercomponent->HP = m_playercomponent->MaxHP;
		else
			m_playercomponent->HP += healthup;

		auto playersound = m_playercomponent->GetComponent<PlayerSoundComponent>();
		if (!GetSceneManager()->GetEntityByIdentityName(playersound->SoundKey_Heal))
			GetSceneManager()->SpawnSoundEntity(playersound->SoundKey_Heal, playersound->Volume_Heal, true, false, {});
	}

}

