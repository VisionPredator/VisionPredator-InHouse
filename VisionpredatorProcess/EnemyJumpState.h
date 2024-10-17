#pragma once
#include "EnemyMovementState.h"

class EnemyJumpState final : public EnemyMovementState
{
public:
	EnemyJumpState() = default;

	void Enter(uint32_t entityID) override;
	void Update(uint32_t entityID, float deltaTime) override;
	void Exit(uint32_t entityID) override;
};

