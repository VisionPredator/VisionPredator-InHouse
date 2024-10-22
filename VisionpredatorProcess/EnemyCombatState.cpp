#include "pch.h"
#include "EnemyCombatState.h"
#include "EnemyIdleCombatState.h"
#include "EnemyRangedAttackState.h"
#include "EnemyHitReactionState.h"

EnemyIdleCombatState EnemyCombatState::s_Idle;
EnemyRangedAttackState EnemyCombatState::s_RangedAttack;

void EnemyCombatState::Enter(const std::shared_ptr<Component>& component)
{
}

void EnemyCombatState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
}

void EnemyCombatState::Exit(const std::shared_ptr<Component>& component)
{
}
