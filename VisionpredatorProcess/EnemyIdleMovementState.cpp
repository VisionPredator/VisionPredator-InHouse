#include "pch.h"
#include "EnemyIdleMovementState.h"

#include "EnemyComponent.h"
#include "../VPGraphics/Log.h"

void EnemyIdleMovementState::Enter(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	Log::GetClientLogger()->info("Enter IdleMovementState");
	Log::GetClientLogger()->info("Success to Get EnemyComponent! Entity ID: {}", enemyComp->GetEntityID());
	Log::GetClientLogger()->info("Entity's HP: {}", enemyComp->HP);
	enemyComp->HP = 30;
	Log::GetClientLogger()->info("Edited Entity's HP: {}", enemyComp->HP);

}

void EnemyIdleMovementState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	Log::GetClientLogger()->info("Update IdleMovementState");
}

void EnemyIdleMovementState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleMovementState");
}
