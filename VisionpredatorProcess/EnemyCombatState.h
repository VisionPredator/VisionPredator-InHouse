#pragma once
#include "EnemyState.h"

class EnemyIdleCombatState;
class EnemyRangedAttackState;
class EnemyHitReactionState;

class EnemyCombatState : public virtual EnemyState
{
public:
	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;

	static EnemyIdleCombatState s_Idle;
	static EnemyRangedAttackState s_RangedAttack;
	static EnemyHitReactionState s_HitReaction;	// 애매한데.
};
