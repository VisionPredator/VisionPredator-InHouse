#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "CollisionCallback.h"
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
}

PhysxEngine::~PhysxEngine()
{
	delete m_Physics;
	PX_RELEASE(m_PxScene);
}

bool PhysxEngine::Initialize()
{
	m_Physics = new Physics;
	m_Collisioncallback = new CollisionCallback;
	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	m_Physics->Initialize();
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());


	sceneDesc.gravity=PxVec3(0.f,9.81f,0.f);
	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
	sceneDesc.solverType = physx::PxSolverType::ePGS;
	sceneDesc.filterShader = CustomSimulationFilterShader;
	sceneDesc.simulationEventCallback = m_Collisioncallback;

	m_PxScene = physics->createScene(sceneDesc);
#ifdef _DEBUG
	m_Physics->SettingPVDClient(m_PxScene);
#endif

	return true;
}

bool PhysxEngine::Finalize()
{
	return false;
}




