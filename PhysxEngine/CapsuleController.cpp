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

void CapsuleController::SetShapeOffset(VPMath::Vector3 offset)
{
	if (m_Controller)
	{
		physx::PxRigidDynamic* body = m_Controller->getActor();
		physx::PxShape* shape = nullptr;
		int shapeCount = body->getNbShapes();
		body->getShapes(&shape, shapeCount);

		if (shape)
		{
			// 로컬 좌표계에서의 오프셋 설정
			physx::PxTransform localPose(physx::PxVec3(offset.x, offset.y, offset.z));
			shape->setLocalPose(localPose);
		}
	}
}

void CapsuleController::UpdateCapsuleDimensions(CapsuleControllerInfo info)
{
	if (m_Controller)
	{
		physx::PxCapsuleController* capsuleController = dynamic_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// 캡슐 크기 업데이트
			capsuleController->setRadius(info.radius);
			capsuleController->setHeight(info.height);
		}
	}
}

void CapsuleController::UpdateCapsuleSize(const CapsuleControllerInfo& newInfo)
{
	if (m_Controller)
	{
		// 현재 캡슐 컨트롤러를 가져오기
		physx::PxCapsuleController* capsuleController = dynamic_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// 기존 캡슐의 높이와 반지름 가져오기
			float originalHeight = capsuleController->getHeight();
			float originalRadius = capsuleController->getRadius();

			// 새로운 높이와 반지름 설정
			float newHeight = newInfo.height;
			float newRadius = newInfo.radius;

			if (originalHeight == newHeight && originalRadius == newRadius)
				return;  // 크기가 동일하므로 함수 종료

			// 높이 차이를 계산
			float heightDifference = originalHeight - newHeight;

			// 오프셋 적용: 바닥이 동일하도록 높이 차이의 절반만큼 Y축 방향으로 이동
			VPMath::Vector3 offset(0.0f, heightDifference / 2.0f, 0.0f);

			// 캡슐의 크기 변경
			UpdateCapsuleDimensions(newInfo);
			// 새 크기에 맞춰 오프셋 적용
			SetShapeOffset(offset);
		}
	}
}


CapsuleController::~CapsuleController()
{
}
