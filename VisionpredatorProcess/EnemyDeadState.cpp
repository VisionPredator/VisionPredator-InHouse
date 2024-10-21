#include "pch.h"
#include "EnemyDeadState.h"
#include "EnemyComponent.h"
#include "RigidBodyComponent.h"
#include "StatesInclude.h"

#include "../VPEngine/SceneManager.h"

void EnemyDeadState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	if (enemyComp->HasComponent<ControllerComponent>())
	{
		enemyComp->PhysicsManager->RemoveController(enemyComp->GetEntityID());
	}

	//ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::DIE, 2.f, 0.f, false);
	ChangeCurrentState(enemyComp, &EnemyCombatState::s_Idle);
	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
}

void EnemyDeadState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyDeadState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit DeadState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
