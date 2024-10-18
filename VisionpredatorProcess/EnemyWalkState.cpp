#include "pch.h"
#include "EnemyWalkState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"

void EnemyWalkState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter WalkState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyWalkState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyWalkState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit WalkState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
