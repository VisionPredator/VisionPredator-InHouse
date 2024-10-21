#include "pch.h"
#include "EnemyHitReactionState.h"

void EnemyHitReactionState::Enter(const std::shared_ptr<Component>& component)
{
	EnemyCombatState::Enter(component);
}

void EnemyHitReactionState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	EnemyCombatState::Update(component, deltaTime);
}

void EnemyHitReactionState::Exit(const std::shared_ptr<Component>& component)
{
	EnemyCombatState::Exit(component);
}
