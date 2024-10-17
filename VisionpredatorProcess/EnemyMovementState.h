#pragma once
#include "IState.h"

class EnemyJumpState;
class EnemyIdleMovementState;
class EnemyWalkState;
class EnemyBackpedalState;
class EnemyRunState;
class EnemyMovementState : public IState
{
public:
	virtual void Enter(uint32_t entityID) override;
	virtual void Update(uint32_t entityID, float deltaTime) override;
	virtual void Exit(uint32_t entityID) override;

public:
	static EnemyIdleMovementState s_Idle;
	static EnemyWalkState s_Walk;
	static EnemyRunState s_Run;
	static EnemyJumpState s_Jumping;
	static EnemyBackpedalState s_Backpedal;
};
