#include "pch.h"
#include "EnemyIdleCombatState.h"

void EnemyIdleCombatState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleCombatState");
}

void EnemyIdleCombatState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{

}

void EnemyIdleCombatState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleCombatState");
}
