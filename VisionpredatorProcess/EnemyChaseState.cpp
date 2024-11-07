#include "pch.h"
#include "EnemyChaseState.h"

#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"
#include "NavAgentComponent.h"

void EnemyChaseState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter ChaseState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Chase;

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();

	if (enemyComp->DistanceToPlayer <= enemyComp->AttackRange)
	{
		ChangeCurrentState(enemyComp, &EnemyMovementState::s_IdleAttack);
		navComp->IsChase = false;
	}
	else
	{
		navComp->IsChase = true;
		ChangeCurrentState(enemyComp, &EnemyMovementState::s_Run);
	}

}

void EnemyChaseState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{	
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	enemyComp->DistanceToPlayer = DetectTarget(*enemyComp, deltaTime);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();

	auto animation = enemyComp->GetComponent<AnimationComponent>();

	// 플레이어로부터 거리가 가까우면
	if (enemyComp->DistanceToPlayer <= enemyComp->AttackRange)
	{
		navComp->IsChase = false;

		if (animation->IsBlending == false)
			if (enemyComp->OnHit == true)
			{
				ChangeCurrentState(enemyComp, &EnemyMovementState::s_HitReaction);
				enemyComp->HeatComplete = false;
			}
			else if (enemyComp->HeatComplete == true)
			{
				ChangeCurrentState(enemyComp, &EnemyMovementState::s_IdleAttack);
			}
	}
	// 플레이어로부터 거리가 멀면
	else
	{
		navComp->IsChase = true;

		if (animation->IsBlending == false)
		{
			if (enemyComp->HeatComplete == true)
			{
				ChangeCurrentState(enemyComp, &EnemyMovementState::s_Run);
			}
		}
	}
}

void EnemyChaseState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit ChaseState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = false;
}
