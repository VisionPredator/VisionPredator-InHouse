#include "pch.h"
#include "CapsuleController.h"
#include "VPPhysicsStructs.h"
#include "ControllerHitCallback.h"

// CapsuleController ������
CapsuleController::CapsuleController() : Controller{}
{
}

// ĸ�� ��Ʈ�ѷ� �ʱ�ȭ �Լ�
bool CapsuleController::Initialize(CapsuleControllerInfo info, physx::PxControllerManager* controllerManager, physx::PxMaterial* material, PhysicsInfo physicinfo)
{
	// �⺻ ��Ʈ�ѷ� �ʱ�ȭ
	ControllerInit(info.Info, material, physicinfo);

	// ĸ�� ��Ʈ�ѷ� ����(Desc) ����
	physx::PxCapsuleControllerDesc desc;
	desc.height = info.height; // ĸ���� ����
	desc.radius = info.radius; // ĸ���� ������
	desc.contactOffset = info.contactOffset; // ���� ������
	desc.stepOffset = info.stepOffset; // ��� ������
	desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING; // ���� �̲����� ���
	desc.slopeLimit = info.slopeLimit; // ��� ����
	desc.maxJumpHeight = 10.f; // �ִ� ���� ����
	desc.position = physx::PxExtendedVec3(info.position.x, info.position.y, info.position.z); // �ʱ� ��ġ ����
	desc.userData = &m_UserData; // ����� ������ ����
	desc.material = material; // ���� ����
	// ��Ʈ �ݹ� ����
	m_ControllerHitCallback = std::make_shared<ControllerHitCallback>();
	desc.reportCallback = dynamic_cast<physx::PxUserControllerHitReport*>(m_ControllerHitCallback.get());
	// ĸ�� ��Ʈ�ѷ� ����
	m_Controller = controllerManager->createController(desc);
	if (!m_Controller)
		return false; // ���� ���� �� false ��ȯ

	// ĸ�� ��Ʈ�ѷ��� RigidBody�� Shape ��������
	physx::PxRigidDynamic* body = m_Controller->getActor();
	if (!body)
		return false; // RigidBody�� ������ ����

	physx::PxShape* shape = nullptr;
	int shapeCount = body->getNbShapes();
	body->getShapes(&shape, shapeCount);
	if (!shape)
		return false; // Shape�� ������ ����

	// ���� �ֹ� �ݺ� Ƚ�� ����
	body->setSolverIterationCounts(8, 4);
	body->userData = &m_UserData; // ����� ������ ����
	shape->userData = &m_UserData; // ����� ������ ����

	// Shape�� ���� �� �޽� ������ ����
	shape->setContactOffset(0.02f);
	shape->setRestOffset(0.01f);

	// �浹 ���� ������ ����
	physx::PxFilterData filterData;
	filterData.word0 = static_cast<int>(info.Info.LayerNumber); // ���� ���̾� ����
	filterData.word1 = physicinfo.CollisionMatrix[static_cast<int>(info.Info.LayerNumber)]; // �浹 ��Ʈ���� ����
	shape->setSimulationFilterData(filterData);

	return true; // �ʱ�ȭ ����
}

// ĸ�� ��Ʈ�ѷ� ũ�� ���� �Լ�
void CapsuleController::ResizeCapsuleControllerSize(float radius, float height)
{
	if (m_Controller)
	{
		// PxCapsuleController�� ĳ����
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);

		if (capsuleController)
		{
			// ���ο� �������� ���� ����
			capsuleController->setRadius(radius);
			capsuleController->setHeight(height);
		}
	}
}

// ĸ�� ũ�⸦ ������Ʈ�ϴ� �Լ�
void CapsuleController::UpdateCapsuleDimensions(CapsuleControllerInfo info)
{
	if (m_Controller)
	{
		// PxCapsuleController�� ĳ����
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// ĸ�� ũ�� ������Ʈ
			capsuleController->setRadius(info.radius);
			capsuleController->setHeight(info.height);
		}
	}
}

// ���ο� ĸ�� ũ�⸦ ������Ʈ�ϴ� �Լ�
void CapsuleController::UpdateCapsuleSize(const CapsuleControllerInfo& newInfo)
{
	if (m_Controller)
	{
		// PxCapsuleController�� ĳ����
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// ���� ĸ�� ũ�⸦ ������
			float originalHeight = capsuleController->getHeight();
			float originalRadius = capsuleController->getRadius();

			// ���ο� ĸ�� ũ��
			float newHeight = newInfo.height;
			float newRadius = newInfo.radius;

			// ũ�Ⱑ ������� ������ ����
			if (originalHeight == newHeight && originalRadius == newRadius)
				return;

			// ĸ�� ũ�� ������Ʈ
			UpdateCapsuleDimensions(newInfo);

			// Pivot�� LocalOffset�� ����Ͽ� Shape ������ ����
			SetShapeOffset(newInfo.Info.Pivot, newInfo.Info.LocalOffset);
		}
	}
}

// CapsuleController �Ҹ���
CapsuleController::~CapsuleController()
{
}

#pragma region Non_use
// Shape �������� �����ϴ� �Լ�
void CapsuleController::SetShapeOffset(ControllerPivot pivot, VPMath::Vector3 offset)
{
	if (m_Controller)
	{
		// PxCapsuleController�� ĳ����
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// ĸ���� ���� ���̿� ������ ��������
			float height = capsuleController->getHeight();
			float radius = capsuleController->getRadius();

			// �⺻ ������ ����
			VPMath::Vector3 shapeOffset = offset;

			// ĸ�� ��ü ���� ��� (������ + ����/2)
			float fullCapsuleHeight = radius + (height * 0.5f);

			// Pivot�� ���� ���� ��� ����
			switch (pivot)
			{
			case ControllerPivot::FOOT:
				// ĸ���� �Ʒ���(�� �κ�)�� ��ġ�� ����
				shapeOffset.y += fullCapsuleHeight;
				break;

			case ControllerPivot::CENTER:
				// �߾� �����̹Ƿ� �߰� ���� ���ʿ�
				break;
			default:
				// ��ȿ���� ���� ��� ó������ ����
				return;
			}

			// Shape�� ���� ���� ������ ����
			physx::PxRigidDynamic* body = m_Controller->getActor();
			physx::PxShape* shape = nullptr;
			int shapeCount = body->getNbShapes();
			body->getShapes(&shape, shapeCount);
			if (shape)
			{
				// ���� ���� ����
				physx::PxTransform localPose(physx::PxVec3(shapeOffset.y, -shapeOffset.x, -shapeOffset.z));
				shape->setLocalPose(localPose);
			}
		}
	}
}
#pragma endregion
