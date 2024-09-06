#pragma once
#include "CommonInclude.h"

class ControllerHitCallback :public physx::PxUserControllerHitReport
{
public:
	ControllerHitCallback();
	~ControllerHitCallback();
	// PxUserControllerHitReport을(를) 통해 상속됨
	void onShapeHit(const PxControllerShapeHit& hit) override;
	void onControllerHit(const PxControllersHit& hit) override;
	void onObstacleHit(const PxControllerObstacleHit& hit) override;
};

