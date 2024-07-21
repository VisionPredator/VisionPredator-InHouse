#include "pch.h"
#include "CapsuleController.h"
#include "VPPhysicsStructs.h"
CapsuleController::CapsuleController():Controller{}
{
}

void CapsuleController::Initialize(CapsuleControllerInfo info, physx::PxControllerManager * CCTManager, PhysicsInfo physicinfo)
{

	ControllerInit(info.Info, physicinfo);
	physx:PxCapsuleControllerDesc desc;

	//CCTManager->createController();
	physicinfo.CollisionMatrix;

}


CapsuleController::~CapsuleController()
{
}
