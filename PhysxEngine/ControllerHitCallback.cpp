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
	// 충돌 시 실행할 코드 작성
	//std::cout << "ShapeHit" << std::endl;
}

void ControllerHitCallback::onControllerHit(const PxControllersHit& hit)
{
	// 다른 컨트롤러와 충돌 시 실행할 코드 작성
	//std::cout << "ControllerHit" << std::endl;
}

void ControllerHitCallback::onObstacleHit(const PxControllerObstacleHit& hit)
{
	// 장애물과 충돌 시 실행할 코드 작성
	//std::cout << "ObstacleHit" << std::endl;
}
