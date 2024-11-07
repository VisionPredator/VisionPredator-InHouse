#include "pch.h"
#include "EnemyIdleAttack.h"
#include "../VPGraphics/Log.h"
#include "EnemyComponent.h"
#include "AnimationComponent.h"

void EnemyIdleAttack::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleAttack");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	ChangeCurrentAnimation(enemyComp, VisPred::Game::EnemyAni::ATTACI_IDLE, 2.0f, 0.2f);
}

void EnemyIdleAttack::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);


}

void EnemyIdleAttack::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleAttack");
}
