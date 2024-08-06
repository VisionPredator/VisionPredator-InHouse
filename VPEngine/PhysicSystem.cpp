#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"
#include "TransformSystem.h"
#include "EventManager.h"
PhysicSystem::PhysicSystem(std::shared_ptr<SceneManager> sceneManager)
	:System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnCollisionExit",CreateSubscriber(&PhysicSystem::OnCollisionExit));
	EventManager::GetInstance().Subscribe("OnCollisionContact",CreateSubscriber(&PhysicSystem::OnCollisionContact));
	EventManager::GetInstance().Subscribe("OnCollisionEnter",CreateSubscriber(&PhysicSystem::OnCollisionEnter));
}

void PhysicSystem::OnCollisionExit(std::any pair)
{
	auto [pairid1, pairid2] = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
	auto entity1 = GetSceneManager()->GetEntity(pairid1).get();
	auto entity2 = GetSceneManager()->GetEntity(pairid2).get();

	if (entity1->HasComponent<GeometryComponent>())
	{
		auto geoComp = entity1->GetComponent<GeometryComponent>();
		geoComp->color = { 0,1,0 };
	}
	if (entity2->HasComponent<GeometryComponent>())
	{
		auto geoComp = entity2->GetComponent<GeometryComponent>();
		geoComp->color = { 0,1,0 };

	}


}

void PhysicSystem::OnCollisionContact(std::any pair)
{
	//auto [pairid1, pairid2] = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
	//auto entity1 =GetSceneManager()->GetEntity(pairid1).get();
	//auto entity2 =GetSceneManager()->GetEntity(pairid2).get();

	//if (entity1->HasComponent<GeometryComponent>())
	//{
	//	auto geoComp = entity1->GetComponent<GeometryComponent>();
	//	geoComp->color = {1,0,0};
	//}
	//if (entity2->HasComponent<GeometryComponent>())
	//{
	//	auto geoComp = entity2->GetComponent<GeometryComponent>();
	//	geoComp->color = { 1,0,0 };

	//}

}

void PhysicSystem::OnCollisionEnter(std::any pair)
{
	auto [pairid1, pairid2] = std::any_cast<std::pair<uint32_t, uint32_t>>(pair);
	auto entity1 = GetSceneManager()->GetEntity(pairid1).get();
	auto entity2 = GetSceneManager()->GetEntity(pairid2).get();

	if (entity1->HasComponent<GeometryComponent>())
	{
		auto geoComp = entity1->GetComponent<GeometryComponent>();
		geoComp->color = { 1,0,0 };
	}
	if (entity2->HasComponent<GeometryComponent>())
	{
		auto geoComp = entity2->GetComponent<GeometryComponent>();
		geoComp->color = { 1,0,0 };

	}
}


void PhysicSystem::Initialize()
{
	m_PhysicsEngine->ApplyPhysicEngineInfo();
	for (TransformComponent& Transform : COMPITER(TransformComponent))
	{
		Start(Transform.GetEntityID());
	}

}

void PhysicSystem::Start(uint32_t EntityID)
{

	CreateRigidBody(EntityID);
	CreateCapsuleController(EntityID);
}

void PhysicSystem::Finish(uint32_t EntityID)
{
	ReleaseRigidBody(EntityID);
	ReleaseCapsuleController(EntityID);

}

void PhysicSystem::Finalize()
{
	for (TransformComponent& rigidBodyComponent : COMPITER(TransformComponent))
	{
		Finish(rigidBodyComponent.GetEntityID());
	}
}

void PhysicSystem::CreateRigidBody(uint32_t EntityID)
{
	
	auto entity = GetSceneManager()->GetEntity(EntityID);
	auto rigidComp = entity->GetComponent<RigidBodyComponent>();
	if (!rigidComp)
		return;
	TransformComponent rigidtransform = *rigidComp->GetComponent<TransformComponent>();
	rigidComp->DefaultColliderInfo.EntityID = EntityID;
	rigidComp->DefaultColliderInfo.WorldLocation = rigidtransform.World_Location;
	rigidComp->DefaultColliderInfo.WorldQuaternion = rigidtransform.World_Quaternion;
	rigidComp->DefaultColliderInfo.WorldScale = rigidtransform.World_Scale;
	if (!rigidComp->IsDynamic)
	{
		switch (rigidComp->ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{
			rigidComp->BoxInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateStaticBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			rigidComp->CapsuleInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateStaticBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			rigidComp->SphereInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateStaticBody(rigidComp->SphereInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CONVEX:
		{
			if (!rigidComp->HasComponent<MeshComponent>())
				return;
			VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			convexMeshResourceInfo.FBXName = rigidComp->GetComponent<MeshComponent>()->FBX;

			if (!m_PhysicsEngine->HasConvexMeshResource(convexMeshResourceInfo.FBXName))
			{
				///TODO GetVertices wstring으로
				std::string temp(convexMeshResourceInfo.FBXName.begin(), convexMeshResourceInfo.FBXName.end());
				convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(temp);
				if (convexMeshResourceInfo.Vertexs.empty()) {
					return; // Return null if no vertices were extracted
				}
				m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
			}
			//rigidComp->SphereInfo.colliderInfo = rigidComp->ColliderInfo;
			VPPhysics::ConvexColliderInfo convexColliderInfo{};
			convexColliderInfo.FBXName = convexMeshResourceInfo.FBXName;
			convexColliderInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateStaticBody(convexColliderInfo, rigidComp->ColliderType);
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
			rigidComp->BoxInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateDynamicBody(rigidComp->BoxInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CAPSULE:
		{
			rigidComp->CapsuleInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateDynamicBody(rigidComp->CapsuleInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			rigidComp->SphereInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateDynamicBody(rigidComp->SphereInfo, rigidComp->ColliderType);
		}
		break;
		case VPPhysics::EColliderShape::CONVEX:
		{
			if (!rigidComp->HasComponent<MeshComponent>())
				return;

			VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			convexMeshResourceInfo.FBXName = rigidComp->GetComponent<MeshComponent>()->FBX;
			

			if (!m_PhysicsEngine->HasConvexMeshResource(convexMeshResourceInfo.FBXName))
			{
				///TODO GetVertices wstring으로
				std::string temp(convexMeshResourceInfo.FBXName.begin(), convexMeshResourceInfo.FBXName.end());
				convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(temp);
				if (convexMeshResourceInfo.Vertexs.empty()) {
					return; // Return null if no vertices were extracted
				}
				m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
			}
			//rigidComp->SphereInfo.colliderInfo = rigidComp->ColliderInfo;
			VPPhysics::ConvexColliderInfo convexColliderInfo{};
			convexColliderInfo.FBXName = convexMeshResourceInfo.FBXName;

			convexColliderInfo.colliderInfo = rigidComp->DefaultColliderInfo;
			m_PhysicsEngine->CreateDynamicBody(convexColliderInfo, rigidComp->ColliderType);
		}
		break;

		default:
			break;
		}
	}
}

void PhysicSystem::CreateCapsuleController(uint32_t EntityID)
{
	auto entity = GetSceneManager()->GetEntity(EntityID);
	auto controllercomp = entity->GetComponent<ControllerComponent>();
	if (!controllercomp)
		return;
	controllercomp->Contollerinfo.EntityId = entity->GetEntityID();
	controllercomp->CapsuleControllerinfo.position = entity->GetComponent<TransformComponent>()->World_Location;
	controllercomp->CapsuleControllerinfo.Info = controllercomp->Contollerinfo;
	m_PhysicsEngine->CreatCapsuleController(controllercomp->CapsuleControllerinfo);


}

void PhysicSystem::ReleaseRigidBody(uint32_t EntityID)
{
	auto entity = GetSceneManager()->GetEntity(EntityID);
	if (!entity->HasComponent<RigidBodyComponent>())
		return;
	m_PhysicsEngine->ReleaseActor(EntityID);

}

void PhysicSystem::ReleaseCapsuleController(uint32_t EntityID)
{
	auto entity = GetSceneManager()->GetEntity(EntityID);
	if (!entity->HasComponent<ControllerComponent>())
		return;
	m_PhysicsEngine->RemoveController(EntityID);
}



void PhysicSystem::RenderUpdate(float deltaTime)
{
	//for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	//{
	//	auto rigidTransform = rigidBodyComponent.GetComponent<TransformComponent>();

	//	switch (rigidBodyComponent.ColliderShape)
	//	{
	//	case VPPhysics::EColliderShape::BOX:
	//	{
	//		debug::OBBInfo temp{};
	//		temp.OBB.Center = rigidTransform->World_Location;



	//		temp.OBB.Extents = rigidBodyComponent.BoxInfo.Extent*rigidTransform->World_Scale;

	//		temp.xAxisAngle = rigidTransform->World_Rotation.x;
	//		temp.yAxisAngle = rigidTransform->World_Rotation.y;
	//		temp.zAxisAngle = rigidTransform->World_Rotation.z;
	//		m_Graphics->DrawOBB(temp);


	//	}
	//	break;
	//	case VPPhysics::EColliderShape::SPHERE:
	//	{
	//		debug::SphereInfo temp{};
	//		temp.Sphere.Center = rigidTransform->World_Location;
	//		float maxscle = rigidTransform->World_Scale.GetMaxComponent();
	//		temp.Sphere.Radius = rigidBodyComponent.SphereInfo.Radius* maxscle;
	//		m_Graphics->DrawSphere(temp);

	//	}
	//	break;
	//	default:
	//		break;
	//	}

	//}
	//for (ControllerComponent& ControllerComp : COMPITER(ControllerComponent))
	//{
	//	auto ControllerTransform = ControllerComp.GetComponent<TransformComponent>();

	//	debug::OBBInfo temp{};
	//	temp.OBB.Center = ControllerTransform->World_Location;
	//	VPPhysics::CapsuleControllerInfo tempinfo = ControllerComp.CapsuleControllerinfo;
	//	temp.OBB.Extents = { tempinfo.radius,(tempinfo.height / 2 + tempinfo.radius),tempinfo.radius };

	//	temp.xAxisAngle = 0;
	//	temp.yAxisAngle = 0;
	//	temp.zAxisAngle = 0;
	//	m_Graphics->DrawOBB(temp);


	//}
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
	
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		
		uint32_t entityID = rigidBodyComponent.GetEntityID();
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;
		auto rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		auto templocation = m_PhysicsEngine->GetGobalLocation(entityID);
		auto offset_T = (rigidBodyTransform->World_Location - templocation).Length();
		VPMath::Quaternion tempQuat = m_PhysicsEngine->GetGobalQuaternion(entityID);
		float offset_R = (rigidBodyTransform->World_Quaternion - tempQuat).Length();
		if (offset_R > m_rotation_threshold_degrees || offset_T > m_location_threshold)
		{
			m_PhysicsEngine->SetGobalPose(entityID, rigidBodyTransform->World_Location, rigidBodyTransform->World_Quaternion);
		}
	}

	for (ControllerComponent& controllerCompoent : COMPITER(ControllerComponent))
	{
		uint32_t entityID = controllerCompoent.GetEntityID();
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;

		TransformComponent* controllerTransform = controllerCompoent.GetComponent<TransformComponent>();
		VPMath::Vector3 templocation = m_PhysicsEngine->GetControllerGobalPose(entityID);
		float offset_T = (controllerTransform->World_Location - templocation).Length();

		if (offset_T > m_location_threshold)
			m_PhysicsEngine->SetControllerGobalPose(entityID, controllerTransform->World_Location);
	}

	m_PhysicsEngine->Update(deltaTime);

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		uint32_t entityID = rigidBodyComponent.GetEntityID();
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;
		auto rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		rigidBodyTransform->World_Location = m_PhysicsEngine->GetGobalLocation(rigidBodyComponent.GetEntityID());
		rigidBodyTransform->World_Quaternion = m_PhysicsEngine->GetGobalQuaternion(rigidBodyComponent.GetEntityID());
	}

	for (ControllerComponent& controllerComponent : COMPITER(ControllerComponent))
	{
		uint32_t entityID = controllerComponent.GetEntityID();
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;
		auto controllerTransform = controllerComponent.GetComponent<TransformComponent>();
		controllerTransform->World_Location = m_PhysicsEngine->GetControllerGobalPose(entityID);
	}



}