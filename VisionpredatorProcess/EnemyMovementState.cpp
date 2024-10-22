#include "pch.h"
#include "EnemyMovementState.h"
#include "EnemyJumpState.h"
#include "EnemyWalkState.h"
#include "EnemyRunState.h"
#include "EnemyBackpedalState.h"
#include "EnemyIdleMovementState.h"
#include "EnemyHitReactionState.h"
#include "../VPGraphics/Log.h"

EnemyJumpState EnemyMovementState::s_Jumping;
EnemyWalkState EnemyMovementState::s_Walk;
EnemyRunState EnemyMovementState::s_Run;
EnemyIdleMovementState EnemyMovementState::s_Idle;
EnemyBackpedalState EnemyMovementState::s_Backpedal;
EnemyHitReactionState EnemyMovementState::s_HitReaction;

void EnemyMovementState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter MovementState");
}

void EnemyMovementState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
}

void EnemyMovementState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit MovementState");
}
