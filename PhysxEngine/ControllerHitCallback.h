#pragma once
class ControllerHitCallback :public physx::PxUserControllerHitReport
{
public:
	ControllerHitCallback();
	~ControllerHitCallback();
	// PxUserControllerHitReport��(��) ���� ��ӵ�
	void onShapeHit(const PxControllerShapeHit& hit) override;
	void onControllerHit(const PxControllersHit& hit) override;
	void onObstacleHit(const PxControllerObstacleHit& hit) override;
};

