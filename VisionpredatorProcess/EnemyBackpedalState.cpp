#include "pch.h"
#include "EnemyBackpedalState.h"

void EnemyBackpedalState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter BackpedalState");
}

void EnemyBackpedalState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{

}

void EnemyBackpedalState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit BackpedalState");
}
