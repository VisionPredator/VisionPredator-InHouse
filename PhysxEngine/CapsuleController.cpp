#include "pch.h"
#include "CapsuleController.h"
#include "VPPhysicsStructs.h"
#include "ControllerHitCallback.h"
CapsuleController::CapsuleController():Controller{}
{
}

bool CapsuleController::Initialize(CapsuleControllerInfo info, physx::PxControllerManager * controllerManager, physx::PxMaterial* material, PhysicsInfo physicinfo)
{
	ControllerInit(info.Info, material, physicinfo);
	physx::PxCapsuleControllerDesc desc;
	desc.height = info.height;
	desc.radius = info.radius;
	desc.contactOffset = info.contactOffset;
	desc.stepOffset = info.stepOffset;
	desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING;
	desc.slopeLimit = info.slopeLimit;
	desc.maxJumpHeight = 10.f;
	desc.position.x = info.position.x;
	desc.position.y = info.position.y;
	desc.position.z = info.position.z;
	desc.material = m_Material;
	m_ControllerHitCallback = std::make_shared<ControllerHitCallback>();
	desc.reportCallback = dynamic_cast<physx::PxUserControllerHitReport*>(m_ControllerHitCallback.get());
	m_Controller= controllerManager->createController(desc);

	physx::PxRigidDynamic* body = m_Controller->getActor();

	int ShapeSize = body->getNbShapes();
	body->userData = &m_EntityID;
	physx::PxShape* shape=nullptr;
	body->getShapes(&shape, ShapeSize);
	body->setSolverIterationCounts(8, 4);
	shape->setContactOffset(0.02f);
	shape->userData = &m_EntityID;
	shape->setRestOffset(0.01f);
	physx::PxFilterData filterData;
	filterData.word0 = (int)info.Info.LayerNumber;
	filterData.word1 = physicinfo.CollisionMatrix[(int)info.Info.LayerNumber];
	shape->setSimulationFilterData(filterData);
	return true;

}


CapsuleController::~CapsuleController()
{
}
