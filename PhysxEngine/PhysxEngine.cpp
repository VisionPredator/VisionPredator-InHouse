#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "CollisionCallback.h"
#include "RigidBodyManager.h"
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
		std::cout << "onContact" << std::endl;

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
	while (m_ElapsedTime> m_UpdateTime)
	{
		m_PxScene->simulate(m_UpdateTime);
		m_PxScene->fetchResults(true);
		m_ElapsedTime -= m_UpdateTime;
	}

}

void PhysxEngine::CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(boxinfo, collidertype, m_EngineInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(sphereinfo, collidertype, m_EngineInfo);
}

void PhysxEngine::CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidManager->CreateStaticBody(capsuleinfo, collidertype, m_EngineInfo);
}

void PhysxEngine::ReleaseActor(uint32_t entityID)
{
	m_RigidManager->ReleaseBodyScene(entityID);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(boxinfo, collidertype, m_EngineInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(sphereinfo, collidertype, m_EngineInfo);
}

void PhysxEngine::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype)
{
	m_RigidManager->CreateDynamicBody(capsuleinfo, collidertype, m_EngineInfo);
}
