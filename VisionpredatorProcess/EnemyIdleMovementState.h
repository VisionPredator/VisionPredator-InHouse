#pragma once
#include "EnemyMovementState.h"

class EnemyIdleMovementState : public EnemyMovementState
{
	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;
};

