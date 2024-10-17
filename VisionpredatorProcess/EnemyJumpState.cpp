#include "pch.h"
#include "EnemyJumpState.h"
#include "../VPGraphics/Log.h"

void EnemyJumpState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter JumpState");
}

void EnemyJumpState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	Log::GetClientLogger()->info("Update JumpState");
}

void EnemyJumpState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit JumpState");
}
