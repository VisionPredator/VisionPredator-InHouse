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

	if (enemyComp->DistanceToPlayer <= enemyComp->AttackRange)
	{
		navComp->IsChase = false;
	}
	else
		navComp->IsChase = true;

	ChangeCurrentState(enemyComp, &EnemyMovementState::s_Run);
}

void EnemyChaseState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	enemyComp->DistanceToPlayer = DetectTarget(*enemyComp, deltaTime);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	
	if (enemyComp->DistanceToPlayer <= enemyComp->AttackRange)
	{
		navComp->IsChase = false;
	}
	else
		navComp->IsChase = true;


}

void EnemyChaseState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit ChaseState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = false;
}
