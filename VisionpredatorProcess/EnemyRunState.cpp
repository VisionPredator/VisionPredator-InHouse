#include "pch.h"
#include "EnemyRunState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"

void EnemyRunState::Enter(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::CHASE, 2.f);
}

void EnemyRunState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::CHASE, 2.f);
}

void EnemyRunState::Exit(const std::shared_ptr<Component>& component)
{
}
