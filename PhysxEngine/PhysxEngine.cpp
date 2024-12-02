#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "DynamicRigidBody.h"
#include "StaticRigidBody.h"
#include "RigidBodyManager.h"
#include "ControllerManager.h"
#include "CollisionCallback.h"
#include "PhysichResourceManager.h"
#include "Controller.h"
#include "EventManager.h"
#include "CollisionManager.h"
#include <iostream>
#include "DataRegister_Physics.h"
#include "../VPGraphics/Log.h"

/// <summary>
/// 충돌 콜백 함수
/// <summary>
physx::PxFilterFlags CustomSimulationFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	/// <summary>
	/// 쌍에 대해 CCD를 활성화하고 초기 및 CCD 연락처에 대한 연락처 보고서를 요청합니다.
	/// 또한 접점별 정보를 제공하고 행위자에게 정보를 제공합니다
	/// 접촉할 때 포즈를 취합니다.
	/// <summary>

	// 필터 셰이더 로직 ( 트리거 )
	if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
	{
		if ((((1 << filterData0.word0) & filterData1.word1) > 0) && (((1 << filterData1.word0) & filterData0.word1) > 0))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT
				| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
				| physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			return physx::PxFilterFlag::eDEFAULT;
		}
	}


	// 필터 데이터 충돌 체크 ( 시뮬레이션 )
 	if ((((1 << filterData0.word0) & filterData1.word1) > 0) && (((1 << filterData1.word0) & filterData0.word1) > 0))
	{

		pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT
			| physx::PxPairFlag::eDETECT_CCD_CONTACT
			| physx::PxPairFlag::eNOTIFY_TOUCH_CCD
			| physx::PxPairFlag::eNOTIFY_TOUCH_FOUND
			| physx::PxPairFlag::eNOTIFY_TOUCH_LOST
			| physx::PxPairFlag::eNOTIFY_CONTACT_POINTS
			| physx::PxPairFlag::eCONTACT_EVENT_POSE
			| physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
		return physx::PxFilterFlag::eDEFAULT;
	}
	else
	{
		pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // 충돌 행동 비허용
		return physx::PxFilterFlag::eSUPPRESS;
	}
}

PhysxEngine::PhysxEngine()
{
	PhysicsRegister::Register_Metadata();
	EventManager::GetInstance().Subscribe("OnSetPhysicInfo", CreateSubscriber(&PhysxEngine::OnSetPhysicInfo));
}

PhysxEngine::~PhysxEngine()
{
	m_ControllerManager = nullptr;
	m_RigidBodyManager = nullptr;
	PX_RELEASE(m_PxScene);
}

bool PhysxEngine::Initialize()
{
	// 물리 엔진의 주요 구성 요소를 생성 및 초기화
	m_Physics = std::make_shared<Physics>();                       // 물리 엔진 객체 생성
	m_Collisioncallback = std::make_shared<CollisionCallback>();   // 충돌 콜백 객체 생성
	m_RigidBodyManager = std::make_shared<RigidBodyManager>();     // 리지드 바디 매니저 생성
	m_ControllerManager = std::make_shared<ControllerManager>();   // 컨트롤러 매니저 생성
	m_RecourceManager = std::make_shared<PhysichResourceManager>(); // 물리 리소스 매니저 생성
	m_CollisionManager = std::make_shared<CollisionManager>();     // 충돌 매니저 생성

	// 물리 엔진 초기화
	m_Physics->Initialize();
	// PhysX Physics 객체 가져오기
	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	// SceneDesc 설정
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale()); // 씬의 물리적 허용 오차 스케일 설정
	sceneDesc.cpuDispatcher = m_Physics->GetDispatcher();        // CPU 디스패처 설정
	sceneDesc.filterShader = CustomSimulationFilterShader;       // 사용자 정의 필터 셰이더 설정
	sceneDesc.simulationEventCallback = m_Collisioncallback.get(); // 충돌 이벤트 콜백 설정
	sceneDesc.gravity = PxVec3(0.f, -9.81f, 0.f);                  // 중력 설정 (y축: -9.81m/s^2)
	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE; // 정적 구조 설정 (AABB Tree)
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;           // CCD(Continuous Collision Detection) 활성화
	sceneDesc.flags |= physx::PxSceneFlag::eDISABLE_CCD_RESWEEP;  // CCD의 Resweep 비활성화
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS; // GPU 기반 물리 동작 활성화 (현재 주석 처리됨)
	//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;    // GPU 브로드 페이즈 활성화 (현재 주석 처리됨)
	sceneDesc.solverType = physx::PxSolverType::ePGS;             // PGS(Pivoting Gauss-Seidel) 솔버 사용

	// PhysX Scene 생성
	m_PxScene = physics->createScene(sceneDesc);
	// 매니저 초기화
	m_RecourceManager->Initialize(m_Physics->GetPxPhysics());
	m_RigidBodyManager->Initialize(m_Physics->GetPxPhysics(), m_PxScene, m_RecourceManager);
	m_ControllerManager->Initialize(m_PxScene, m_Physics->GetPxPhysics(), m_CollisionManager.get());

#ifdef _DEBUG
	// 디버그 모드에서 PVD(PhysX Visual Debugger) 클라이언트 설정
	m_Physics->SettingPVDClient(m_PxScene);
#endif

	return true; // 초기화 성공 반환
}



bool PhysxEngine::Finalize()
{
	return false;
}

void PhysxEngine::Update(float deltatime)
{

	m_ControllerManager->Update(deltatime);
	SimulateUpdate(deltatime);
	m_CollisionManager->Update();
	/// 충돌 체크 관런
}

void PhysxEngine::SimulateUpdate(float deltatime)
{

	m_PxScene->simulate(deltatime);
	m_PxScene->fetchResults(true);

}

void PhysxEngine::OnSetPhysicInfo(std::any data)
{
	VPPhysics::PhysicsInfo tempPhysicsInfo = std::any_cast<VPPhysics::PhysicsInfo>(data);
	SetPhysicsInfo(tempPhysicsInfo);

}

void PhysxEngine::ConvertToStaticWithLayer(uint32_t entityID, EPhysicsLayer layer)
{
	m_RigidBodyManager->ConvertToStaticWithLayer(entityID, layer);
	m_CollisionManager->RemoveEntity(entityID);
	Log::GetCoreLogger()->warn("Changed RigidBody!!");

}

void PhysxEngine::ConvertToDynamicWithLayer(uint32_t entityID, EPhysicsLayer layer)
{
	m_RigidBodyManager->ConvertToDynamicWithLayer(entityID, layer);
	m_CollisionManager->RemoveEntity(entityID);
	Log::GetCoreLogger()->warn("Changed RigidBody!!");
}

void PhysxEngine::ConvertToStatic(uint32_t entityID)
{
	m_RigidBodyManager->ConvertToStatic(entityID);
	m_CollisionManager->RemoveEntity(entityID);
	Log::GetCoreLogger()->warn("Changed RigidBody!!");
}

void PhysxEngine::ConvertToDynamic(uint32_t entityID)
{
	m_RigidBodyManager->ConvertToDynamic(entityID);
	m_CollisionManager->RemoveEntity(entityID);
	Log::GetCoreLogger()->warn("Changed RigidBody!!");
}



void PhysxEngine::ExtractVerticesAndFacesByLayer(EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	m_RigidBodyManager->ExtractSceneVerticesAndFacesByLayer(m_PxScene, layer, outVertices, outIndices);
}

void PhysxEngine::ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{

	m_RigidBodyManager->ExtractEntityVerticesAndFaces(entityID, outVertices, outIndices);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateStaticBody(boxinfo, collidertype, m_PhyiscsInfo);
}	

void PhysxEngine::CreateStaticBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateStaticBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateStaticBody(capsuleinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateStaticBody(convexinfo, collidertype, m_PhyiscsInfo);

}
void PhysxEngine::ReleaseActor(uint32_t entityID)
{
	m_RigidBodyManager->ReleaseBodyScene(entityID);
	m_CollisionManager->RemoveEntity(entityID);
	Log::GetCoreLogger()->warn("Released RigidBody!!");
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(boxinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(capsuleinfo, collidertype, m_PhyiscsInfo);
}


void PhysxEngine::CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(convexinfo, collidertype, m_PhyiscsInfo);

}

bool PhysxEngine::HasRigidBody(uint32_t entityID)
{
	return m_RigidBodyManager->HasRigidBody(entityID);;
}

bool PhysxEngine::HasController(uint32_t entityID)
{
	return m_ControllerManager->HasController(entityID);
}

RaycastData PhysxEngine::RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
	if (m_RigidBodyManager->HasRigidBody(entityID))
		return m_RigidBodyManager->RaycastActor(entityID, dir, distance);
	else if (m_ControllerManager->HasController(entityID))
		return 	m_ControllerManager->RaycastActor(entityID, dir, distance);
	return {};
}

RaycastData PhysxEngine::RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
	if (m_RigidBodyManager->HasRigidBody(entityID))
		return m_RigidBodyManager->RaycastActor_Offset(entityID, offset, dir, distance);
	else if (m_ControllerManager->HasController(entityID))
		return m_ControllerManager->RaycastActor_Offset(entityID, offset, dir, distance);
	return {};
}

RaycastData PhysxEngine::RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return m_RigidBodyManager->RaycastActorAtPose(location, dir, distance);
}
RaycastData PhysxEngine::RaycastActorAtPose_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return  m_RigidBodyManager->RaycastActorAtPose_Ignore(entityID, location, dir, distance);
}

RaycastData PhysxEngine::RaycastActorAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return  m_RigidBodyManager->RaycastActorAtPose_Ignores(entityIDs, location, dir, distance);
}


void PhysxEngine::ResizeCapsuleController(uint32_t entityID, float radius, float height)
{
	if (m_ControllerManager->HasController(entityID))
	{
		m_ControllerManager->ResizeCapsuleControllerSize(entityID, radius, height);
	}
}

std::vector<RaycastData> PhysxEngine::RaycastActorsAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return m_RigidBodyManager->RaycastActorsAtPose(location, dir, distance);
}

std::vector<RaycastData> PhysxEngine::RaycastActorsAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return m_RigidBodyManager->RaycastActorsAtPose_Ignores(entityIDs, location, dir, distance);
}

bool PhysxEngine::IsDynamic(uint32_t entityID)
{
	return m_RigidBodyManager->IsDynamic(entityID);
}

bool PhysxEngine::IsStatic(uint32_t entityID)
{
	return m_RigidBodyManager->IsStatic(entityID);
}

std::vector<VPMath::Vector3> PhysxEngine::GetConVexMeshVertex(uint32_t entityID)
{
	return m_RigidBodyManager->GetConVexMeshVertex(entityID);
}


void PhysxEngine::SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q)
{
	m_RigidBodyManager->SetGobalPose(entityID, P, Q);
}

VPMath::Vector3 PhysxEngine::GetGobalLocation(uint32_t entityID)
{
	return m_RigidBodyManager->GetGobalLocation(entityID);
}

VPMath::Quaternion PhysxEngine::GetGobalQuaternion(uint32_t entityID)
{
	return m_RigidBodyManager->GetGobalQuaternion(entityID);

}

void PhysxEngine::UpdatePhysicEngineInfo()
{
	//m_EngineInfo;
	if (!m_PxScene)
		return;
	m_PxScene->setGravity({ m_PhyiscsInfo.Gravity.x,m_PhyiscsInfo.Gravity.y,m_PhyiscsInfo.Gravity.z });
}

void PhysxEngine::ApplyPhysicEngineInfo()
{
	UpdatePhysicEngineInfo();
}

void PhysxEngine::AddVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity)
{
	m_RigidBodyManager->AddVelocity(entityID, Dir, velocity);
}
void PhysxEngine::SetVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity)
{
	m_RigidBodyManager->SetVelocity(entityID, Dir, velocity);
}

void PhysxEngine::AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power)
{
}

VPMath::Vector3 PhysxEngine::GetVelocity(uint32_t entityID)
{

	return m_RigidBodyManager->GetVelocity(entityID);

}

void PhysxEngine::CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo)
{

	m_ControllerManager->CreatCapsuleController(capsuleinfo,m_PhyiscsInfo);
}

void PhysxEngine::RemoveController(uint32_t entityID)
{
	m_ControllerManager->RemoveController(entityID);
	Log::GetCoreLogger()->warn("Released CapsuleController!!");
}

void PhysxEngine::SetControllerGobalPose(uint32_t entityID, VPMath::Vector3 P)
{
	auto Controller = m_ControllerManager->GetController(entityID);
	if (!Controller)
		return;
	Controller->SetPosition(P);
}


VPMath::Vector3 PhysxEngine::GetControllerGobalPose(uint32_t entityID)
{
	auto Controller = m_ControllerManager->GetController(entityID);
	if (!Controller)
		return {0,0,0};
	return Controller->GetPosition();
}

void PhysxEngine::SetControllerVelocity(uint32_t entityID, VPMath::Vector3 velocity)
{
	if (auto controller =m_ControllerManager->GetController(entityID))
	{
		controller->SetVelocity(velocity);
	}
}

bool PhysxEngine::GetControllerIsFall(uint32_t entityID)
{
	if (auto controller = m_ControllerManager->GetController(entityID))
	{
	return controller->GetIsFall();
	}
	return false;
}

void PhysxEngine::LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info)
{
	m_RecourceManager->LoadConvexMeshResource(info);
}

bool PhysxEngine::HasConvexMeshResource(const std::wstring& key)
{
	return 	m_RecourceManager->HasConvexMeshResource(key);
}

