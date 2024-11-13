#include "pch.h"
#include "EnemyRunState.h"
#include "EnemyComponent.h"
#include "EnemySoundComponent.h"
#include "StatesInclude.h"
#include "TransformComponent.h"
#include "../VPGraphics/Log.h"

void EnemyRunState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter RunState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentAnimation(*enemyComp, VisPred::Game::EnemyAni::CHASE, 2.f, 0.2f);
}

void EnemyRunState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	const auto soundEngine = enemyComp->SoundEngine;

	if (!enemyComp->HasComponent<EnemySoundComponent>())
		return; 

	const auto soundComp = enemyComp->GetComponent<EnemySoundComponent>();
	// 사운드가 재생중이라면 플레이하지 않음
	if (!soundEngine->IsPlayingSound(soundComp->GetEntityID(), soundComp->SoundKey_Run))
	{
		soundEngine->Stop(soundComp->GetEntityID());
		soundEngine->Play(
			soundComp->GetEntityID(), 
			soundComp->SoundKey_Run, 
			soundComp->Volume_Run, 
			false, 
			false, 
			soundComp->GetComponent<TransformComponent>()->World_Location
		);
	}
}

void EnemyRunState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RunState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
