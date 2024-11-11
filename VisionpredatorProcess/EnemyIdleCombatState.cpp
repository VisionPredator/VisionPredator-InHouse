#include "pch.h"
#include "EnemyIdleCombatState.h"
#include "EnemyComponent.h"

void EnemyIdleCombatState::Enter(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Enter IdleCombatState");
	auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
}

void EnemyIdleCombatState::Update(const std::shared_ptr<Component>& component, float deltaTime)
{
	const auto enemyComp = std::dynamic_pointer_cast<EnemyComponent>(component);
	if (CheckIsDead(enemyComp))
		return;
	enemyComp->DistanceToPlayer = DetectTarget(*enemyComp, deltaTime);

	// 플레이어가 공격 사정거리 안에 있다면 공격 상태로 전환
	if (enemyComp->DistanceToPlayer >= 0.0f && enemyComp->DistanceToPlayer <= enemyComp->MinRangedAttackRange)
		ChangeCurrentState(enemyComp, &EnemyCombatState::s_RangedAttack);

}

void EnemyIdleCombatState::Exit(const std::shared_ptr<Component>& component)
{
	Log::GetClientLogger()->info("Exit IdleCombatState");
}
