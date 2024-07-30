#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "DynamicRigidBody.h"
#include "StaticRigidBody.h"
#include "RigidBodyManager.h"
#include "ControllerManager.h"
#include "CollisionCallback.h"
#include "Controller.h"
#include "../VPEngine/EventManager.h"
#include <iostream>
/// <summary>
/// �浹 �ݹ� �Լ�
/// <summary>
physx::PxFilterFlags CustomSimulationFilterShader(
	physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	/// <summary>
	/// �ֿ� ���� CCD�� Ȱ��ȭ�ϰ� �ʱ� �� CCD ����ó�� ���� ����ó ������ ��û�մϴ�.
	/// ���� ������ ������ �����ϰ� �����ڿ��� ������ �����մϴ�
	/// ������ �� ��� ���մϴ�.
	/// <summary>

	// ���� ���̴� ���� ( Ʈ���� )
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

	// ���� ������ �浹 üũ ( �ùķ��̼� )
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
		pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // �浹 �ൿ �����
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
	m_Collisioncallback = new CollisionCallback;
	m_RigidBodyManager = new RigidBodyManager;
	m_ControllerManager = new ControllerManager;

	m_Physics->Initialize();
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
	sceneDesc.solverType = physx::PxSolverType::ePGS;
	m_PxScene = physics->createScene(sceneDesc);

	m_RigidBodyManager->Initialize(m_Physics->GetPxPhysics(), m_PxScene, m_CollisionManager);
	m_ControllerManager->Initialize(m_PxScene,m_Physics->GetPxPhysics(), m_CollisionManager);
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
	m_UpdateTime = (1.f / GetPhysicsInfo().FrameRate);
	bool IsUpdated = false;
	while (m_ElapsedTime >= m_UpdateTime)
	{
		IsUpdated = true;
		m_ControllerManager->Update(m_UpdateTime);
		SimulateUpdate(m_UpdateTime);
		m_ElapsedTime -= m_UpdateTime;
	}
	if (IsUpdated && m_ElapsedTime > 0)
	{
		m_ControllerManager->Update(m_ElapsedTime);
		SimulateUpdate(m_ElapsedTime);

	}


	/// �浹 üũ ����
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

void PhysxEngine::CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateStaticBody(boxinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateStaticBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateStaticBody(capsuleinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::ReleaseActor(uint32_t entityID)
{
	m_RigidBodyManager->ReleaseBodyScene(entityID);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(boxinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(sphereinfo, collidertype, m_PhyiscsInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidBodyManager->CreateDynamicBody(capsuleinfo, collidertype, m_PhyiscsInfo);
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
