#include "pch.h"
#include "EnemyIdleState.h"
#include "EnemyComponent.h"

void EnemyIdleState::Enter(const std::shared_ptr<Component>& component)
{
	
}

void EnemyIdleState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);
}

void EnemyIdleState::Exit(const std::shared_ptr<Component>& component)
{
}
