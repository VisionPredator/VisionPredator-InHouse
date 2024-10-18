#include "pch.h"
#include "EnemyChaseState.h"
#include "EnemyComponent.h"

void EnemyChaseState::Enter(const std::shared_ptr<Component>& component)
{
}

void EnemyChaseState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);
}

void EnemyChaseState::Exit(const std::shared_ptr<Component>& component)
{
}
