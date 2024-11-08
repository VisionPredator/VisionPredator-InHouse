#include "pch.h"
#include "EnemyIdleState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"
#include "TransformComponent.h"
#include "SceneManager.h"

void EnemyIdleState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Idle;

	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Idle);
}

void EnemyIdleState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	DetectTarget(*enemyComp, deltaTime);
}

void EnemyIdleState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
