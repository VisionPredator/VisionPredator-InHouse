#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"
#include "CollisionCallback.h"
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




