#pragma once
#include "IState.h"

class EnemyState : public IState
{
public:
	~EnemyState() override = default;

	void Enter(uint32_t entityID) override;
	void Update(uint32_t entityID, float deltaTime) override;
	void Exit(uint32_t entityID) override;
};

