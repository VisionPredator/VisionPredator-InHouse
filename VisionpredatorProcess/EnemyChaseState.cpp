#include "pch.h"
#include "EnemyChaseState.h"
#include "EnemyComponent.h"
#include "NavAgentComponent.h"
#include "StatesInclude.h"

struct null_deleter
{
	void operator()(void const*) const {}
};

void EnemyChaseState::Enter(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);

	auto navComp = enemyComp->GetComponent<NavAgentComponent>();
	navComp->IsChase = true;

	// ¸¶¹«¸®
	enemyComp->MovementState = &EnemyMovementState::s_Run;
	enemyComp->MovementState->Enter(enemyComp);
}

void EnemyChaseState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	DetectTarget(*enemyComp, deltaTime);
}

void EnemyChaseState::Exit(const std::shared_ptr<Component>& component)
{
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}
