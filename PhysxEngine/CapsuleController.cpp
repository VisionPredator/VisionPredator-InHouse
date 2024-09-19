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
			// ���� ��ǥ�迡���� ������ ����
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
			// ĸ�� ũ�� ������Ʈ
			capsuleController->setRadius(info.radius);
			capsuleController->setHeight(info.height);
		}
	}
}

void CapsuleController::UpdateCapsuleSize(const CapsuleControllerInfo& newInfo)
{
	if (m_Controller)
	{
		// ���� ĸ�� ��Ʈ�ѷ��� ��������
		physx::PxCapsuleController* capsuleController = dynamic_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// ���� ĸ���� ���̿� ������ ��������
			float originalHeight = capsuleController->getHeight();
			float originalRadius = capsuleController->getRadius();

			// ���ο� ���̿� ������ ����
			float newHeight = newInfo.height;
			float newRadius = newInfo.radius;

			if (originalHeight == newHeight && originalRadius == newRadius)
				return;  // ũ�Ⱑ �����ϹǷ� �Լ� ����

			// ���� ���̸� ���
			float heightDifference = originalHeight - newHeight;

			// ������ ����: �ٴ��� �����ϵ��� ���� ������ ���ݸ�ŭ Y�� �������� �̵�
			VPMath::Vector3 offset(0.0f, heightDifference / 2.0f, 0.0f);

			// ĸ���� ũ�� ����
			UpdateCapsuleDimensions(newInfo);
			// �� ũ�⿡ ���� ������ ����
			SetShapeOffset(offset);
		}
	}
}


CapsuleController::~CapsuleController()
{
}
