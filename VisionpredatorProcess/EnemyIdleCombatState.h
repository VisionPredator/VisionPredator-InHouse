#pragma once
#include "EnemyCombatState.h"

class EnemyIdleCombatState final : public EnemyCombatState
{
public:
	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;
};

