#include "pch.h"
#include "EnemyPatrolState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"

void EnemyPatrolState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter PatrolState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyPatrolState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyPatrolState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit PatrolState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
