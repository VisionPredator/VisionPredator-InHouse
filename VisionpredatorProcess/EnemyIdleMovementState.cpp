#include "pch.h"
#include "EnemyIdleMovementState.h"
#include "../VPGraphics/Log.h"

void EnemyIdleMovementState::Enter(uint32_t entityID)
{
	Log::GetClientLogger()->info("Enter IdleMovementState");


}

void EnemyIdleMovementState::Update(uint32_t entityID, float deltaTime)
{
	Log::GetClientLogger()->info("Update IdleMovementState");
}

void EnemyIdleMovementState::Exit(uint32_t entityID)
{
	Log::GetClientLogger()->info("Exit IdleMovementState");
}
