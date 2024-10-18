#include "pch.h"
#include "EnemyPatrolState.h"

void EnemyPatrolState::Enter(const std::shared_ptr<Component>& component)
{
	EnemyBehaviorState::Enter(component);
}

void EnemyPatrolState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	EnemyBehaviorState::Update(component, deltaTime);
}

void EnemyPatrolState::Exit(const std::shared_ptr<Component>& component)
{
	EnemyBehaviorState::Exit(component);
}
