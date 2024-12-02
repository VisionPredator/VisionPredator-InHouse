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
	// ���� ������ �ֿ� ���� ��Ҹ� ���� �� �ʱ�ȭ
	m_Physics = std::make_shared<Physics>();                       // ���� ���� ��ü ����
	m_Collisioncallback = std::make_shared<CollisionCallback>();   // �浹 �ݹ� ��ü ����
	m_RigidBodyManager = std::make_shared<RigidBodyManager>();     // ������ �ٵ� �Ŵ��� ����
	m_ControllerManager = std::make_shared<ControllerManager>();   // ��Ʈ�ѷ� �Ŵ��� ����
	m_RecourceManager = std::make_shared<PhysichResourceManager>(); // ���� ���ҽ� �Ŵ��� ����
	m_CollisionManager = std::make_shared<CollisionManager>();     // �浹 �Ŵ��� ����

	// ���� ���� �ʱ�ȭ
	m_Physics->Initialize();
	// PhysX Physics ��ü ��������
	physx::PxPhysics* physics = m_Physics->GetPxPhysics();
	// SceneDesc ����
	physx::PxSceneDesc sceneDesc(physics->getTolerancesScale()); // ���� ������ ��� ���� ������ ����
	sceneDesc.cpuDispatcher = m_Physics->GetDispatcher();        // CPU ����ó ����
	sceneDesc.filterShader = CustomSimulationFilterShader;       // ����� ���� ���� ���̴� ����
	sceneDesc.simulationEventCallback = m_Collisioncallback.get(); // �浹 �̺�Ʈ �ݹ� ����
	sceneDesc.gravity = PxVec3(0.f, -9.81f, 0.f);                  // �߷� ���� (y��: -9.81m/s^2)
	sceneDesc.staticStructure = physx::PxPruningStructureType::eDYNAMIC_AABB_TREE; // ���� ���� ���� (AABB Tree)
	sceneDesc.flags |= physx::PxSceneFlag::eENABLE_CCD;           // CCD(Continuous Collision Detection) Ȱ��ȭ
	sceneDesc.flags |= physx::PxSceneFlag::eDISABLE_CCD_RESWEEP;  // CCD�� Resweep ��Ȱ��ȭ
	//sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS; // GPU ��� ���� ���� Ȱ��ȭ (���� �ּ� ó����)
	//sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;    // GPU ��ε� ������ Ȱ��ȭ (���� �ּ� ó����)
	sceneDesc.solverType = physx::PxSolverType::ePGS;             // PGS(Pivoting Gauss-Seidel) �ֹ� ���

	// PhysX Scene ����
	m_PxScene = physics->createScene(sceneDesc);
	// �Ŵ��� �ʱ�ȭ
	m_RecourceManager->Initialize(m_Physics->GetPxPhysics());
	m_RigidBodyManager->Initialize(m_Physics->GetPxPhysics(), m_PxScene, m_RecourceManager);
	m_ControllerManager->Initialize(m_PxScene, m_Physics->GetPxPhysics(), m_CollisionManager.get());

#ifdef _DEBUG
	// ����� ��忡�� PVD(PhysX Visual Debugger) Ŭ���̾�Ʈ ����
	m_Physics->SettingPVDClient(m_PxScene);
#endif

	return true; // �ʱ�ȭ ���� ��ȯ
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

