#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"
#include "TransformSystem.h"

PhysicSystem::PhysicSystem(SceneManager* sceneManager)
	:System(sceneManager)
{
}


void PhysicSystem::Initialize()
{
	m_PhysicsEngine->ApplyPhysicEngineInfo();
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
	rigidComp->ColliderInfo.WorldLocation = rigidComp->GetComponent<TransformComponent>()->World_Location;
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
			m_PhysicsEngine->CreateStaticBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			m_PhysicsEngine->CreateStaticBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);

		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			m_PhysicsEngine->CreateStaticBody(rigidComp->SphereInfo, rigidComp->ColliderType);

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
			m_PhysicsEngine->CreateDynamicBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			m_PhysicsEngine->CreateDynamicBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			m_PhysicsEngine->CreateDynamicBody(rigidComp->SphereInfo, rigidComp->ColliderType);
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
	m_PhysicsEngine->ReleaseActor(gameObjectId);
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
			temp.OBB.Center = rigidTransform->World_Location;
			temp.OBB.Extents = rigidBodyComponent.BoxInfo.Extent;

			temp.xAxisAngle = rigidTransform->World_Rotation.x;
			temp.yAxisAngle = rigidTransform->World_Rotation.y;
			temp.zAxisAngle = rigidTransform->World_Rotation.z;
			m_Graphics->DrawOBB(temp);


		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			debug::SphereInfo temp{};
			temp.Sphere.Center = rigidTransform->World_Location;
			temp.Sphere.Radius = rigidBodyComponent.SphereInfo.Radius;
			m_Graphics->DrawSphere(temp);

		}
		break;
		default:
			break;
		}

	}

}
// Function to compute the conjugate of a quaternion
VPMath::Quaternion QuaternionConjugate(const VPMath::Quaternion& q)
{
	return VPMath::Quaternion(-q.x, -q.y, -q.z, q.w);
}
float QuaternionAngleDifference(const VPMath::Quaternion& q1, const VPMath::Quaternion& q2)
{

	VPMath::Quaternion delta = q1 * QuaternionConjugate(q2);
	delta.Normalize(); // Ensure the quaternion is normalized

	// Clamp the w component to be within the range of -1.0 to 1.0
	float angleInRadians = 2.0f * std::acos(std::clamp(delta.w, -1.0f, 1.0f));
	return angleInRadians * (180.0f / VPMath::XM_PI); // Convert to degrees
}


void PhysicSystem::PhysicsUpdate(float deltaTime)
{
	EventManager::GetInstance().ImmediateEvent("OnUpdateTransfomData");
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		uint32_t entityID = rigidBodyComponent.GetEntityID();
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();

		auto templocation = m_PhysicsEngine->GetGobalLocation(entityID);
		auto offset_T = (rigidBodyTransform->World_Location - templocation).Length();

		VPMath::Quaternion tempQuat = m_PhysicsEngine->GetGobalQuaternion(entityID);
		float offset_R = (rigidBodyTransform->World_Quaternion - tempQuat).Length();
		if (offset_R > m_rotation_threshold_degrees || offset_T > m_location_threshold)
		{
			m_PhysicsEngine->SetGobalPose(entityID, rigidBodyTransform->World_Location, rigidBodyTransform->World_Quaternion);
		}
	}
	m_PhysicsEngine->Update(deltaTime);

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		TransformComponent* rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		rigidBodyTransform->World_Location = m_PhysicsEngine->GetGobalLocation(rigidBodyComponent.GetEntityID());
		rigidBodyTransform->World_Quaternion = m_PhysicsEngine->GetGobalQuaternion(rigidBodyComponent.GetEntityID());
	}
	EventManager::GetInstance().ImmediateEvent("OnUpdateTransfomData");


}
