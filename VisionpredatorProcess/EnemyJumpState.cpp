#include "pch.h"
#include "EnemyJumpState.h"
#include "../VPGraphics/Log.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"

void EnemyJumpState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter JumpState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyJumpState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyJumpState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit JumpState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
