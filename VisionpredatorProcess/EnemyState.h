#pragma once
#include "IState.h"

class EnemyState : public virtual IState
{
public:
	~EnemyState() override = default;

	void Enter(const std::shared_ptr<Component>& component) override;
	void Update(const std::shared_ptr<Component>& component, float deltaTime) override;
	void Exit(const std::shared_ptr<Component>& component) override;
};

