#include "pch.h"
#include "EnemyHitReactionState.h"

#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "NavAgentComponent.h"

#include "../VPGraphics/Log.h"

void EnemyHitReactionState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter HitReactionState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::ATTACKED, 3.5f, 0.05f, false, true);
}

void EnemyHitReactionState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	enemyComp->GetComponent<NavAgentComponent>()->IsChase = false;

	if (enemyComp->GetComponent<AnimationComponent>()->IsFinished)
	{
		ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Chase);
	}
}

void EnemyHitReactionState::Exit(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	Log::GetClientLogger()->info("Exit HitReactionState");
}
