#include "pch.h"
#include "ControllerHitCallback.h"
#include <iostream>

ControllerHitCallback::ControllerHitCallback()
{
}

ControllerHitCallback::~ControllerHitCallback()
{
}

void ControllerHitCallback::onShapeHit(const PxControllerShapeHit& hit)
{
	// �浹 �� ������ �ڵ� �ۼ�
	//std::cout << "ShapeHit" << std::endl;
}

void ControllerHitCallback::onControllerHit(const PxControllersHit& hit)
{
	// �ٸ� ��Ʈ�ѷ��� �浹 �� ������ �ڵ� �ۼ�
	//std::cout << "ControllerHit" << std::endl;
}

void ControllerHitCallback::onObstacleHit(const PxControllerObstacleHit& hit)
{
	// ��ֹ��� �浹 �� ������ �ڵ� �ۼ�
	//std::cout << "ObstacleHit" << std::endl;
}
