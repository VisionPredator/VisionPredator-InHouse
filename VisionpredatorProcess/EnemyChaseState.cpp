#include "pch.h"
#include "EnemyChaseState.h"

#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "StatesInclude.h"
#include "NavAgentComponent.h"

void EnemyChaseState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter ChaseState");
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	enemyComp->CurrentFSM = VisPred::Game::EnemyStates::Chase;

	const auto navComp = enemyComp->GetComponent<NavAgentComponent>();

	if (enemyComp->DistanceToPlayer <= enemyComp->ReachableDistanceToPlayer)
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
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	enemyComp->DistanceToPlayer = DetectTarget(*enemyComp, deltaTime);

	const auto navComp = enemyComp->GetComponent<NavAgentComponent>();

	const auto animationComp = enemyComp->GetComponent<AnimationComponent>();

	// 플레이어로부터 거리가 가까우면
	if (enemyComp->DistanceToPlayer <= enemyComp->ReachableDistanceToPlayer)
	{
		navComp->IsChase = false;

		if (animationComp->IsBlending == false)
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

		if (animationComp->IsBlending == false)
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
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	const auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = false;
}
