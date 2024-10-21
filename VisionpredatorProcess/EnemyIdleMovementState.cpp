#include "pch.h"
#include "EnemyIdleMovementState.h"

#include "EnemyComponent.h"
#include "EnemyDeadState.h"
#include "../VPGraphics/Log.h"

#include "SceneManager.h"

void EnemyIdleMovementState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleMovementState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	if(dynamic_cast<EnemyDeadState*>(enemyComp->BehaviorState) == &EnemyBehaviorState::s_Dead)
		ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::DIE, 2.f, 0.f, false);
	else
		ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::IDLE, 2.f);
}

void EnemyIdleMovementState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyIdleMovementState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleMovementState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
