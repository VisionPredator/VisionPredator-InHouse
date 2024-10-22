#include "pch.h"
#include "EnemyChaseState.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"
#include "NavAgentComponent.h"

void EnemyChaseState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter ChaseState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = true;

	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Run);
}

void EnemyChaseState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	DetectTarget(*enemyComp, deltaTime);

	// 일정시간 마다 플레이어를 공격
	// 명중률에 따라 피격 정도를 다르게한다.
}

void EnemyChaseState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit ChaseState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = false;
}
