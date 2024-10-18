#include "pch.h"
#include "EnemyIdleMovementState.h"

#include "EnemyComponent.h"
#include "../VPGraphics/Log.h"

#include "SceneManager.h"

void EnemyIdleMovementState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleMovementState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

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
