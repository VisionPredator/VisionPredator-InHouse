#pragma once
#include "EnemyBehaviorState.h"

class EnemyDeadState final : public EnemyBehaviorState
{
public:
	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;
};

