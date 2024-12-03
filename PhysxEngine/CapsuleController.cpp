#include "pch.h"
#include "CapsuleController.h"
#include "VPPhysicsStructs.h"
#include "ControllerHitCallback.h"

// CapsuleController 생성자
CapsuleController::CapsuleController() : Controller{}
{
}

// 캡슐 컨트롤러 초기화 함수
bool CapsuleController::Initialize(CapsuleControllerInfo info, physx::PxControllerManager* controllerManager, physx::PxMaterial* material, PhysicsInfo physicinfo)
{
	// 기본 컨트롤러 초기화
	ControllerInit(info.Info, material, physicinfo);

	// 캡슐 컨트롤러 설명(Desc) 설정
	physx::PxCapsuleControllerDesc desc;
	desc.height = info.height; // 캡슐의 높이
	desc.radius = info.radius; // 캡슐의 반지름
	desc.contactOffset = info.contactOffset; // 접촉 오프셋
	desc.stepOffset = info.stepOffset; // 계단 오프셋
	desc.nonWalkableMode = physx::PxControllerNonWalkableMode::ePREVENT_CLIMBING_AND_FORCE_SLIDING; // 경사면 미끄러짐 모드
	desc.slopeLimit = info.slopeLimit; // 경사 제한
	desc.maxJumpHeight = 10.f; // 최대 점프 높이
	desc.position = physx::PxExtendedVec3(info.position.x, info.position.y, info.position.z); // 초기 위치 설정
	desc.userData = &m_UserData; // 사용자 데이터 연결
	desc.material = material; // 재질 연결
	// 히트 콜백 설정
	m_ControllerHitCallback = std::make_shared<ControllerHitCallback>();
	desc.reportCallback = dynamic_cast<physx::PxUserControllerHitReport*>(m_ControllerHitCallback.get());
	// 캡슐 컨트롤러 생성
	m_Controller = controllerManager->createController(desc);
	if (!m_Controller)
		return false; // 생성 실패 시 false 반환

	// 캡슐 컨트롤러의 RigidBody와 Shape 가져오기
	physx::PxRigidDynamic* body = m_Controller->getActor();
	if (!body)
		return false; // RigidBody가 없으면 실패

	physx::PxShape* shape = nullptr;
	int shapeCount = body->getNbShapes();
	body->getShapes(&shape, shapeCount);
	if (!shape)
		return false; // Shape가 없으면 실패

	// 물리 솔버 반복 횟수 설정
	body->setSolverIterationCounts(8, 4);
	body->userData = &m_UserData; // 사용자 데이터 연결
	shape->userData = &m_UserData; // 사용자 데이터 연결

	// Shape의 접촉 및 휴식 오프셋 설정
	shape->setContactOffset(0.02f);
	shape->setRestOffset(0.01f);

	// 충돌 필터 데이터 설정
	physx::PxFilterData filterData;
	filterData.word0 = static_cast<int>(info.Info.LayerNumber); // 필터 레이어 설정
	filterData.word1 = physicinfo.CollisionMatrix[static_cast<int>(info.Info.LayerNumber)]; // 충돌 매트릭스 설정
	shape->setSimulationFilterData(filterData);

	return true; // 초기화 성공
}

// 캡슐 컨트롤러 크기 조정 함수
void CapsuleController::ResizeCapsuleControllerSize(float radius, float height)
{
	if (m_Controller)
	{
		// PxCapsuleController로 캐스팅
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);

		if (capsuleController)
		{
			// 새로운 반지름과 높이 설정
			capsuleController->setRadius(radius);
			capsuleController->setHeight(height);
		}
	}
}

// 캡슐 크기를 업데이트하는 함수
void CapsuleController::UpdateCapsuleDimensions(CapsuleControllerInfo info)
{
	if (m_Controller)
	{
		// PxCapsuleController로 캐스팅
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// 캡슐 크기 업데이트
			capsuleController->setRadius(info.radius);
			capsuleController->setHeight(info.height);
		}
	}
}

// 새로운 캡슐 크기를 업데이트하는 함수
void CapsuleController::UpdateCapsuleSize(const CapsuleControllerInfo& newInfo)
{
	if (m_Controller)
	{
		// PxCapsuleController로 캐스팅
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// 현재 캡슐 크기를 가져옴
			float originalHeight = capsuleController->getHeight();
			float originalRadius = capsuleController->getRadius();

			// 새로운 캡슐 크기
			float newHeight = newInfo.height;
			float newRadius = newInfo.radius;

			// 크기가 변경되지 않으면 리턴
			if (originalHeight == newHeight && originalRadius == newRadius)
				return;

			// 캡슐 크기 업데이트
			UpdateCapsuleDimensions(newInfo);

			// Pivot과 LocalOffset을 사용하여 Shape 오프셋 적용
			SetShapeOffset(newInfo.Info.Pivot, newInfo.Info.LocalOffset);
		}
	}
}

// CapsuleController 소멸자
CapsuleController::~CapsuleController()
{
}

#pragma region Non_use
// Shape 오프셋을 설정하는 함수
void CapsuleController::SetShapeOffset(ControllerPivot pivot, VPMath::Vector3 offset)
{
	if (m_Controller)
	{
		// PxCapsuleController로 캐스팅
		physx::PxCapsuleController* capsuleController = static_cast<physx::PxCapsuleController*>(m_Controller);
		if (capsuleController)
		{
			// 캡슐의 현재 높이와 반지름 가져오기
			float height = capsuleController->getHeight();
			float radius = capsuleController->getRadius();

			// 기본 오프셋 설정
			VPMath::Vector3 shapeOffset = offset;

			// 캡슐 전체 높이 계산 (반지름 + 높이/2)
			float fullCapsuleHeight = radius + (height * 0.5f);

			// Pivot에 따라 로컬 포즈를 조정
			switch (pivot)
			{
			case ControllerPivot::FOOT:
				// 캡슐의 아래쪽(발 부분)을 위치에 정렬
				shapeOffset.y += fullCapsuleHeight;
				break;

			case ControllerPivot::CENTER:
				// 중앙 정렬이므로 추가 조정 불필요
				break;
			default:
				// 유효하지 않은 경우 처리하지 않음
				return;
			}

			// Shape의 로컬 포즈 오프셋 적용
			physx::PxRigidDynamic* body = m_Controller->getActor();
			physx::PxShape* shape = nullptr;
			int shapeCount = body->getNbShapes();
			body->getShapes(&shape, shapeCount);
			if (shape)
			{
				// 로컬 포즈 적용
				physx::PxTransform localPose(physx::PxVec3(shapeOffset.y, -shapeOffset.x, -shapeOffset.z));
				shape->setLocalPose(localPose);
			}
		}
	}
}
#pragma endregion
