#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "CollisionCallback.h"
#include "RigidBodyManager.h"
#include "../VPEngine/EventManager.h"
#include "DynamicRigidBody.h"
#include "StaticRigidBody.h"
#include <iostream>
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
	EventManager::GetInstance().Subscribe("OnSetPhysicInfo", CreateSubscriber(&PhysxEngine::OnSetPhysicInfo));
}

PhysxEngine::~PhysxEngine()
{
	PX_RELEASE(m_PxScene);
	delete m_Physics;
}

bool PhysxEngine::Initialize()
{
	m_Physics = new Physics;
	m_Physics->Initialize();
	m_Collisioncallback = new CollisionCallback;

	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	sceneDesc.cpuDispatcher = m_Physics->GetDispatcher();
	sceneDesc.filterShader = CustomSimulationFilterShader;
	sceneDesc.simulationEventCallback = m_Collisioncallback;
	sceneDesc.gravity=PxVec3(0.f,-9.81f,0.f);
	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;
	sceneDesc.flags |= physx::PxSceneFlag::eDISABLE_CCD_RESWEEP;
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
	//sceneDesc.solverType = physx::PxSolverType::ePGS;
	m_PxScene = physics->createScene(sceneDesc);
	m_RigidManager = new RigidBodyManager;
	m_RigidManager->Initialize(m_Physics->GetPxPhysics(), m_PxScene, m_CollisionManager);
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
	m_ElapsedTime += deltatime;
	m_UpdateTime = (float)1 / GetPhysicsInfo().FrameRate;
	bool IsUpdated = false;
	while (m_ElapsedTime > m_UpdateTime)
	{
		m_PxScene->simulate(m_UpdateTime);
		m_PxScene->fetchResults(true);
		m_ElapsedTime -= m_UpdateTime;
		IsUpdated = true;
	}


}

void PhysxEngine::OnSetPhysicInfo(std::any data)
{
	VPPhysics::PhysicsInfo tempPhysicsInfo = std::any_cast<VPPhysics::PhysicsInfo>(data);
	SetPhysicsInfo(tempPhysicsInfo);

}

void PhysxEngine::CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(boxinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(capsuleinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::ReleaseActor(uint32_t entityID)
{
	m_RigidManager->ReleaseBodyScene(entityID);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(boxinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(capsuleinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q)
{
	m_RigidManager->SetGobalPose(entityID, P, Q);
}

VPMath::Vector3 PhysxEngine::GetGobalLocation(uint32_t entityID)
{
	return m_RigidManager->GetGobalLocation(entityID);
}

VPMath::Quaternion PhysxEngine::GetGobalQuaternion(uint32_t entityID)
{
	return m_RigidManager->GetGobalQuaternion(entityID);

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
	m_RigidManager->AddVelocity(entityID, Dir, velocity);
}

void PhysxEngine::AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power)
{
}

VPMath::Vector3 PhysxEngine::GetVelocity(uint32_t entityID)
{

	return m_RigidManager->GetVelocity(entityID);

}
