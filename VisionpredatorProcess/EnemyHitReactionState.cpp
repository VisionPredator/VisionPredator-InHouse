#include "pch.h"
#include "EnemyHitReactionState.h"

#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "EnemySoundComponent.h"
#include "NavAgentComponent.h"
#include "SceneManager.h"
#include "../VPGraphics/Log.h"

void EnemyHitReactionState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter HitReactionState");
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::ATTACKED, 3.5f, 0.05f, false, true);

	const auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();
	//enemyComp->SceneManager.lock()->SpawnSoundEntity(
	//	soundComp->SoundKey_Hurt, 
	//	soundComp->Volume_Hurt, 
	//	false, 
	//	false, 
	//	enemyComp->GetComponent<TransformComponent>()->World_Location
	//);

	// 이미 재생되고 있었다면 끊고 다시 재생한다.
	const auto soundEngine = enemyComp->SoundEngine;
	soundEngine->Stop(soundComp->GetEntityID());
	soundEngine->Play(
		soundComp->GetEntityID(), 
		soundComp->SoundKey_Hurt, 
		soundComp->Volume_Hurt, 
		false, 
		false, 
		soundComp->GetComponent<TransformComponent>()->World_Location
	);

	enemyComp->HeatComplete = false;
}

void EnemyHitReactionState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	enemyComp->GetComponent<NavAgentComponent>()->IsChase = false;

	const auto animation = enemyComp->GetComponent<AnimationComponent>();
	if (animation->IsFinished)
	{
		ChangeCurrentState(enemyComp, &EnemyBehaviorState::s_Chase);
		enemyComp->HeatComplete = true;

		enemyComp->OnHit = false;
	}
}

void EnemyHitReactionState::Exit(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	Log::GetClientLogger()->info("Exit HitReactionState");

	enemyComp->OnHit = false;
}
