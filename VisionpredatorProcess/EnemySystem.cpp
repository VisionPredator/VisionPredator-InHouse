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

void EnemySystem::Initialize()
{
	PlayerComponent* playerComponent = nullptr;

	COMPLOOP(PlayerComponent, comp)
	{
		uint32_t playerID = comp.GetEntityID();
		if (GetSceneManager()->HasComponent<PlayerComponent>(playerID))
		{
			playerComponent = GetSceneManager()->GetComponent<PlayerComponent>(playerID);
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

		comp.Player = playerComponent;
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

	enemyCompRawPtr->SceneManager = m_SceneManager;
	enemyCompRawPtr->PhysicsManager = m_PhysicsEngine;
	enemyCompRawPtr->Graphics = m_Graphics;
	enemyComp->BehaviorState->Enter(enemyComp);
	//enemyComp->CombatState->Enter(enemyComp);
	enemyComp->MovementState->Enter(enemyComp);
}

void EnemySystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(EnemyComponent, enemycomp)
	{
		const std::shared_ptr<EnemyComponent> enemyComp(&enemycomp, null_deleter{});	// null_deleter를 사용해 메모리 해제가 되지 않도록 스마트 포인터 생성

		enemycomp.BehaviorState->Update(enemyComp, deltaTime);
		//enemycomp.CombatState->Update(enemyComp, deltaTime);
		enemycomp.MovementState->Update(enemyComp, deltaTime);

		//Log::GetClientLogger()->info("Current Behavior State: {}", std::string(typeid(*enemycomp.BehaviorState).name()).substr(6));
		//Log::GetClientLogger()->info("Current Movement State: {}", std::string(typeid(*enemycomp.MovementState).name()).substr(6));
	}
}

