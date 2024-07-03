#include "pch.h"
#include "PhysxEngine.h"
#include "Physics.h"


PhysxEngine::PhysxEngine()
{
}

PhysxEngine::~PhysxEngine()
{
	delete m_Physics;
	 m_PxScene->release();
}

void PhysxEngine::Initialize()
{
	m_Physics = new Physics;

	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	m_Physics->Initialize();
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale());
	m_PxScene = physics->createScene(sceneDesc);
	m_Physics->SettingPVDClient(m_PxScene);


	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
	sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
	sceneDesc.solverType = physx::PxSolverType::eTGS;


}

