#include "pch.h"
#include "EnemyJumpState.h"
#include "../VPGraphics/Log.h"

void EnemyJumpState::Enter(uint32_t entityID)
{
	Log::GetClientLogger()->info("Enter JumpState");
}

void EnemyJumpState::Update(uint32_t entityID, float deltaTime)
{
	Log::GetClientLogger()->info("Update JumpState");
}

void EnemyJumpState::Exit(uint32_t entityID)
{
	Log::GetClientLogger()->info("Exit JumpState");
}
