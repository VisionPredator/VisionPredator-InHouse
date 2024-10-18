#pragma once
#include "EnemyState.h"

class EnemyDeadState;
class EnemyPatrolState;
class EnemyChaseState;
class EnemyIdleState;

class EnemyBehaviorState : public virtual EnemyState
{
public:
	virtual void Enter(const std::shared_ptr<Component>& component) override;
	virtual void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	virtual void Exit(const std::shared_ptr<Component>& component) override;

public:
	static EnemyIdleState s_Idle;
	static EnemyChaseState s_Chase;
	static EnemyPatrolState s_Patrol;
	static EnemyDeadState s_Dead;
};

