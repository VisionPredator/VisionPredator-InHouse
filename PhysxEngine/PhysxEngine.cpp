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
	m_Physics =std::make_shared<Physics>();
	m_Collisioncallback =std::make_shared<CollisionCallback>();
	m_RigidBodyManager = std::make_shared<RigidBodyManager>();
	m_ControllerManager =std::make_shared<ControllerManager>();
	m_RecourceManager= std::make_shared<PhysichResourceManager>();
	m_CollisionManager = std::make_shared<CollisionManager>();
	m_Physics->Initialize();
	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.cpuDispatcher = m_Physics->GetDispatcher();
	sceneDesc.filterShader = CustomSimulationFilterShader;
	sceneDesc.simulationEventCallback = m_Collisioncallback.get();
	sceneDesc.gravity=PxVec3(0.f,-9.81f,0.f);
	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.flags |= physx::PxSceneFlag::eDISABLE_CCD_RESWEEP;
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
	sceneDesc.solverType = physx::PxSolverType::ePGS;
	m_PxScene = physics->createScene(sceneDesc);
	m_RecourceManager->Initialize(m_Physics->GetPxPhysics());
	m_RigidBodyManager->Initialize(m_Physics->GetPxPhysics(), m_PxScene, m_RecourceManager);
	m_ControllerManager->Initialize(m_PxScene,m_Physics->GetPxPhysics(), m_CollisionManager.get());
#ifdef _DEBUG
	m_Physics->SettingPVDClient(m_PxScene);
#endif

	return true;
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

void PhysxEngine::ExtractVerticesAndFacesByLayer(EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	m_RigidBodyManager->ExtractSceneVerticesAndFacesByLayer(m_PxScene, layer, outVertices, outIndices);
}

void PhysxEngine::ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{

	m_RigidBodyManager->ExtractVerticesAndFaces(entityID, outVertices, outIndices);
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

uint32_t PhysxEngine::RaycastToHitActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
	if (m_RigidBodyManager->HasRigidBody(entityID))
		return m_RigidBodyManager->RaycastToHitActor(entityID, dir, distance);
	else if (m_ControllerManager->HasController(entityID))
		return 	m_ControllerManager->RaycastToHitActor(entityID, dir, distance);
	return 0;
}

uint32_t PhysxEngine::RaycastToHitActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
	if (m_RigidBodyManager->HasRigidBody(entityID))
		return m_RigidBodyManager->RaycastToHitActor_Offset(entityID, offset, dir, distance);
	else if (m_ControllerManager->HasController(entityID))
		return m_ControllerManager->RaycastToHitActor_Offset(entityID, offset, dir, distance);
	return 0;
}

uint32_t PhysxEngine::RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return m_RigidBodyManager->RaycastToHitActorFromLocation(location, dir, distance);
}
uint32_t PhysxEngine::RaycastToHitActorFromLocation_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	return  m_RigidBodyManager->RaycastToHitActorFromLocation_Ignore(entityID, location, dir, distance);
}


void PhysxEngine::ResizeCapsuleController(uint32_t entityID, float radius, float height)
{
	if (m_ControllerManager->HasController(entityID))
	{
		m_ControllerManager->ResizeCapsuleControllerSize(entityID, radius, height);
	}
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
		return {};
	return Controller->GetPosition();
}

void PhysxEngine::SetControllerVelocity(uint32_t entityID, VPMath::Vector3 velocity)
{
	m_ControllerManager->GetController(entityID)->SetVelocity(velocity);
}

bool PhysxEngine::GetControllerIsFall(uint32_t entityID)
{
	return m_ControllerManager->GetController(entityID)->GetIsFall();
}

void PhysxEngine::LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info)
{
	m_RecourceManager->LoadConvexMeshResource(info);
}

bool PhysxEngine::HasConvexMeshResource(const std::wstring& key)
{
	return 	m_RecourceManager->HasConvexMeshResource(key);
}

