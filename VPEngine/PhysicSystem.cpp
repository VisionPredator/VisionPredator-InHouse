#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"

PhysicSystem::PhysicSystem(SceneManager* sceneManager)
	:System(sceneManager)
{
}

void PhysicSystem::Update(float deltaTime)
{

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
	}

}

void PhysicSystem::FixedUpdate(float deltaTime)
{

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
	}
	m_PhysxEngine->Update(deltaTime);
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
	}

}
void PhysicSystem::Initialize()
{
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		Start(rigidBodyComponent.GetEntityID());
	}
}

void PhysicSystem::Start(uint32_t gameObjectId)
{
	Entity* entity = m_SceneManager->GetEntity(gameObjectId);
	RigidBodyComponent* rigidComp = entity->GetComponent<RigidBodyComponent>();
	rigidComp->ColliderInfo;
	rigidComp->ColliderInfo.EntityID = gameObjectId;
	rigidComp->ColliderInfo.WorldLocation= rigidComp->GetComponent<TransformComponent>()->World_Location;
	rigidComp->ColliderInfo.WorldQuaternion = rigidComp->GetComponent<TransformComponent>()->World_Quaternion;
	rigidComp->BoxInfo.colliderInfo = rigidComp->ColliderInfo;
	rigidComp->CapsuleInfo.colliderInfo = rigidComp->ColliderInfo;
	rigidComp->SphereInfo.colliderInfo = rigidComp->ColliderInfo;
	if (!rigidComp->IsDynamic)
	{
		switch (rigidComp->ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{
			m_PhysxEngine->CreateStaticBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			m_PhysxEngine->CreateStaticBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);

		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			m_PhysxEngine->CreateStaticBody(rigidComp->SphereInfo, rigidComp->ColliderType);

		}
		break;
		default:
			break;
		}
	}
	else
	{
		switch (rigidComp->ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{
			m_PhysxEngine->CreateDynamicBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			m_PhysxEngine->CreateDynamicBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			m_PhysxEngine->CreateDynamicBody(rigidComp->SphereInfo, rigidComp->ColliderType);
		}
		break;
		default:
			break;
		}
	}



}

void PhysicSystem::Finish(uint32_t gameObjectId)
{
	Entity* entity = m_SceneManager->GetEntity(gameObjectId);
	if (!entity->HasComponent<RigidBodyComponent>())
		return;
	m_PhysxEngine->ReleaseActor(gameObjectId);
}

void PhysicSystem::Finalize()
{
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		Finish(rigidBodyComponent.GetEntityID());
	}
}

void PhysicSystem::RenderUpdate(float deltaTime)
{
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		
		switch (rigidBodyComponent.ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{
			debug::OBBInfo temp{};
			temp.OBB.Center= rigidTransform->World_Location;
			temp.OBB.Extents= rigidBodyComponent.BoxInfo.Extent;
			temp.OBB.Orientation= rigidTransform->World_Quaternion;
			m_Graphics->DrawOBB(temp);


		}
			break;
		default:
			break;
		}
	
	}

}
