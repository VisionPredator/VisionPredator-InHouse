#include "pch.h"
#include "EnemyPatrolState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"

void EnemyPatrolState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter PatrolState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Patrol;

	// TODO: 추격 중 플레이어를 놓쳤을 때 탐색하는 기능 구현
}

void EnemyPatrolState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	DetectTarget(*enemyComp, deltaTime);
}

void EnemyPatrolState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit PatrolState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
