#include "pch.h"
#include "EnemyDeadState.h"
#include "EnemyComponent.h"
#include "RigidBodyComponent.h"
#include "StatesInclude.h"
#include "EventManager.h"
#include "../VPEngine/SceneManager.h"

void EnemyDeadState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Dead;

	if (enemyComp->HasComponent<ControllerComponent>())
	{
		enemyComp->PhysicsManager->RemoveController(enemyComp->GetEntityID());
	}
	EventManager::GetInstance().ImmediateEvent("OnRemoveNavAgent", enemyComp->GetEntityID());
	ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
	enemyComp->MovementState->Enter(enemyComp);
	EventManager::GetInstance().ImmediateEvent("OnUpdateSector", enemyComp->GetEntityID());
}

void EnemyDeadState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	constexpr float timeToDelete = 20.f;

	enemyComp->DeadTime += deltaTime;

	if (enemyComp->DeadTime >= timeToDelete)
	{
		enemyComp->SceneManager.lock()->DeleteEntity(enemyComp->GetEntityID());
		Log::GetClientLogger()->info("Destroy Enemy");
	}
}

void EnemyDeadState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
