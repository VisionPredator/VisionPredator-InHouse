#pragma once
#include "EnemyState.h"

class EnemyJumpState;
class EnemyIdleMovementState;
class EnemyWalkState;
class EnemyBackpedalState;
class EnemyRunState;
class EnemyHitReactionState;
class EnemyMovementState : public virtual EnemyState
{
public:
	virtual void Enter(const std::shared_ptr<Component>& component) override;
	virtual void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	virtual void Exit(const std::shared_ptr<Component>& component) override;

public:
	static EnemyIdleMovementState s_Idle;
	static EnemyWalkState s_Walk;
	static EnemyRunState s_Run;
	static EnemyJumpState s_Jumping;
	static EnemyBackpedalState s_Backpedal;
	static EnemyHitReactionState s_HitReaction;	// 애매한데.
};
