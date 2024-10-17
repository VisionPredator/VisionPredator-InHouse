#include "pch.h"
#include "EnemyMovementState.h"
#include "EnemyJumpState.h"
#include "EnemyWalkState.h"
#include "EnemyRunState.h"
#include "EnemyBackpedalState.h"
#include "EnemyIdleMovementState.h"
#include "../VPGraphics/Log.h"

EnemyJumpState EnemyMovementState::s_Jumping;
EnemyWalkState EnemyMovementState::s_Walk;
EnemyRunState EnemyMovementState::s_Run;
EnemyIdleMovementState EnemyMovementState::s_Idle;
EnemyBackpedalState EnemyMovementState::s_Backpedal;

void EnemyMovementState::Enter(uint32_t entityID)
{
	Log::GetClientLogger()->info("Enter MovementState");
}

void EnemyMovementState::Update(uint32_t entityID, float deltaTime)
{
	Log::GetClientLogger()->info("Update MovementState");
}

void EnemyMovementState::Exit(uint32_t entityID)
{
	Log::GetClientLogger()->info("Exit MovementState");
}
