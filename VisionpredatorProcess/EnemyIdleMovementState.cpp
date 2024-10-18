#include "pch.h"
#include "EnemyIdleMovementState.h"

#include "EnemyComponent.h"
#include "../VPGraphics/Log.h"

#include "SceneManager.h"

void EnemyIdleMovementState::Enter(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	Log::GetClientLogger()->info("Enter IdleMovementState");
}

void EnemyIdleMovementState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
}

void EnemyIdleMovementState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleMovementState");
}
