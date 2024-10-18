#include "pch.h"
#include "EnemyBehaviorState.h"

#include "EnemyChaseState.h"
#include "EnemyDeadState.h"
#include "EnemyIdleState.h"
#include "EnemyPatrolState.h"

EnemyIdleState EnemyBehaviorState::s_Idle;
EnemyChaseState EnemyBehaviorState::s_Chase;
EnemyPatrolState EnemyBehaviorState::s_Patrol;
EnemyDeadState EnemyBehaviorState::s_Dead;

void EnemyBehaviorState::Enter(const std::shared_ptr<Component>& component)
{
}

void EnemyBehaviorState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
}

void EnemyBehaviorState::Exit(const std::shared_ptr<Component>& component)
{
}
