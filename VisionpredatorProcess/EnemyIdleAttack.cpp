#include "pch.h"
#include "EnemyIdleAttack.h"
#include "../VPGraphics/Log.h"
#include "EnemyComponent.h"

void EnemyIdleAttack::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleAttack");

}

void EnemyIdleAttack::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyIdleAttack::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleAttack");
}
