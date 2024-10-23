#include "pch.h"
#include "EnemyRangedAttackState.h"

void EnemyRangedAttackState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter RangedAttackState");
}

void EnemyRangedAttackState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{

}

void EnemyRangedAttackState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit RangedAttackState");
}
