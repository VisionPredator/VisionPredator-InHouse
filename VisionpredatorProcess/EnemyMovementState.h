#pragma once
#include "EnemyState.h"

class EnemyAttackState;
class EnemyIdleMovementState;
class EnemyIdleAttack;
class EnemyWalkState;
class EnemyRunState;
class EnemyJumpState;
class EnemyBackpedalState;
class EnemyHitReactionState;

class EnemyMovementState : public virtual EnemyState
{
public:
	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;

public:
	static EnemyIdleMovementState s_Idle;
	static EnemyIdleAttack s_IdleAttack;
	static EnemyWalkState s_Walk;
	static EnemyRunState s_Run;
	static EnemyJumpState s_Jumping;
	static EnemyBackpedalState s_Backpedal;
	static EnemyHitReactionState s_HitReaction;
	static EnemyAttackState s_Attack;
};
