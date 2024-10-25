#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"
#include "TransformSystem.h"
#include "EventManager.h"
#include "../VPGraphics/Log.h"

PhysicSystem::PhysicSystem(std::shared_ptr<SceneManager> sceneManager)
	:System(sceneManager)
{
}

#pragma region IStartable
void PhysicSystem::Initialize()
{
	auto physicInfo = GetSceneManager()->GetScenePhysic();
	m_PhysicsEngine->SetPhysicsInfo(physicInfo);
	EventManager::GetInstance().ImmediateEvent("OnSetPhysicUpdateRate", physicInfo.FrameRate);
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
#pragma endregion


void PhysicSystem::PhysicsUpdate(float deltaTime)
{
	
}
void PhysicSystem::PhysicsLateUpdate(float deltaTime)
{
	EventManager::GetInstance().ImmediateEvent("OnUpdate");

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
		rigidBodyComponent.Speed = m_PhysicsEngine->GetVelocity(rigidBodyTransform->GetEntityID());
	}

	for (ControllerComponent& controllerCompoent : COMPITER(ControllerComponent))
	{
		if (!m_PhysicsEngine->HasController(controllerCompoent.GetEntityID()))
			continue;
		uint32_t entityID = controllerCompoent.GetEntityID();

		TransformComponent* controllerTransform = controllerCompoent.GetComponent<TransformComponent>();

		VPMath::Vector3 templocation = m_PhysicsEngine->GetControllerGobalPose(entityID);
		templocation = DisApplyPivotAndOffset(controllerCompoent, templocation);
		float offset_T = (controllerTransform->World_Location - templocation).Length();
		if (offset_T > m_location_threshold)
		{
			templocation = ApplyPivotAndOffset(controllerCompoent, controllerTransform->World_Location);
			m_PhysicsEngine->SetControllerGobalPose(entityID, templocation);
		}

	}

	m_PhysicsEngine->Update(deltaTime);

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		uint32_t entityID = rigidBodyComponent.GetEntityID();
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;
		auto rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();

		rigidBodyTransform->SetWorldLocation(m_PhysicsEngine->GetGobalLocation(entityID));
		rigidBodyTransform->SetWorldQuaternion(m_PhysicsEngine->GetGobalQuaternion(entityID));
		rigidBodyComponent.Speed = m_PhysicsEngine->GetVelocity(rigidBodyTransform->GetEntityID());
	}

	for (ControllerComponent& controllerComponent : COMPITER(ControllerComponent))
	{
		if (!m_PhysicsEngine->HasController(controllerComponent.GetEntityID()))
			continue;
		uint32_t entityID = controllerComponent.GetEntityID();

		auto controllerTransform = controllerComponent.GetComponent<TransformComponent>();
		auto templocation = m_PhysicsEngine->GetControllerGobalPose(entityID);
		templocation = DisApplyPivotAndOffset(controllerComponent, templocation);
		controllerTransform->SetWorldLocation(templocation);
		TransformSystem::AddUpdateData(controllerTransform);
	}
	EventManager::GetInstance().ImmediateEvent("OnUpdate");
}
#pragma region Physics Fuction
VPMath::Vector3 PhysicSystem::ApplyPivotAndOffset(const ControllerComponent& controllerComponent, VPMath::Vector3 baseLocation)
{
	VPMath::Vector3 adjustedLocation = baseLocation + controllerComponent.Contollerinfo.LocalOffset;

	switch (controllerComponent.Contollerinfo.Pivot)
	{
	case VPPhysics::ControllerPivot::FOOT:
		adjustedLocation.y += (controllerComponent.CapsuleControllerinfo.radius + controllerComponent.CapsuleControllerinfo.height / 2);
		break;
	default:
		break;
	}

	return adjustedLocation;
}
VPMath::Vector3 PhysicSystem::DisApplyPivotAndOffset(const ControllerComponent& controllerComponent, VPMath::Vector3 baseLocation)
{
	VPMath::Vector3 adjustedLocation = baseLocation - controllerComponent.Contollerinfo.LocalOffset;

	switch (controllerComponent.Contollerinfo.Pivot)
	{
	case VPPhysics::ControllerPivot::FOOT:
		adjustedLocation.y -= (controllerComponent.CapsuleControllerinfo.radius + controllerComponent.CapsuleControllerinfo.height / 2);
		break;
	default:
		break;
	}

	return adjustedLocation;
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
#pragma endregion
#pragma region Render
void PhysicSystem::BeginRenderUpdate(float deltaTime)
{
	debug::OBBInfo obbInfo{};
	debug::SphereInfo sphereInfo{};

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		auto rigidTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		//RightVector
		debug::RayInfo frontInfo{};
		frontInfo.Color = { 0,0,1,1 };
		frontInfo.Origin = rigidTransform->World_Location;
		frontInfo.Direction = 10 * rigidTransform->FrontVector;
		frontInfo.Normalize = false;
		//m_Graphics->DrawRay(frontInfo);

		switch (rigidBodyComponent.ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{


			obbInfo.OBB.Center = rigidTransform->World_Location;
			obbInfo.OBB.Extents = rigidBodyComponent.BoxInfo.Extent * rigidTransform->World_Scale;
			obbInfo.xAxisAngle = rigidTransform->World_Rotation.x;
			obbInfo.yAxisAngle = rigidTransform->World_Rotation.y;
			obbInfo.zAxisAngle = rigidTransform->World_Rotation.z;
			m_Graphics->DrawOBB(obbInfo);


		}
		break;
		case VPPhysics::EColliderShape::SPHERE:
		{
			sphereInfo.Sphere.Center = rigidTransform->World_Location;
			float maxscle = rigidTransform->World_Scale.GetMaxComponent();
			sphereInfo.Sphere.Radius = rigidBodyComponent.SphereInfo.Radius * maxscle;
			m_Graphics->DrawSphere(sphereInfo);

		}
		break;
		default:
			break;
		}

	}
	for (ControllerComponent& ControllerComp : COMPITER(ControllerComponent))
	{
		auto ControllerTransform = ControllerComp.GetComponent<TransformComponent>();

		debug::OBBInfo obbInfo{};
		VPPhysics::CapsuleControllerInfo tempinfo = ControllerComp.CapsuleControllerinfo;
		obbInfo.OBB.Center = ControllerTransform->World_Location;
		 if(ControllerComp.Contollerinfo.Pivot == ControllerPivot::FOOT)
			 obbInfo.OBB.Center.y += (tempinfo.height / 2 + tempinfo.radius);


		obbInfo.OBB.Extents = { tempinfo.radius,(tempinfo.height / 2 + tempinfo.radius),tempinfo.radius };

		obbInfo.xAxisAngle = ControllerTransform->World_Rotation.x;
		obbInfo.yAxisAngle = ControllerTransform->World_Rotation.y;
		obbInfo.zAxisAngle = ControllerTransform->World_Rotation.z;
		m_Graphics->DrawOBB(obbInfo);

		//FrontVector
		debug::RayInfo frontinfo{};
		frontinfo.Color = { 0,0,1,1 };
		frontinfo.Origin = ControllerTransform->World_Location;
		frontinfo.Direction = 10 * ControllerTransform->FrontVector;
		frontinfo.Normalize = false;

		m_Graphics->DrawRay(frontinfo);

		//RightVector
		debug::RayInfo rightinfo{};
		rightinfo.Color = { 1,0,0,1 };
		rightinfo.Origin = ControllerTransform->World_Location;
		rightinfo.Direction = 10 * ControllerTransform->RightVector;
		rightinfo.Normalize = false;
		//m_Graphics->DrawRay(rightinfo);
	}
}

void PhysicSystem::RenderUpdate(float deltaTime)
{
}
void PhysicSystem::LateRenderUpdate(float deltaTime)
{
}
void PhysicSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}
#pragma endregion
#pragma region Create


void PhysicSystem::CreateRigidBody(uint32_t EntityID)
{

	auto entity = GetSceneManager()->GetEntity(EntityID);
	if (!entity->HasComponent<RigidBodyComponent>())
		return;
	auto rigidComp = entity->GetComponent<RigidBodyComponent>();
	const TransformComponent& rigidtransform = *rigidComp->GetComponent<TransformComponent>();
	rigidComp->DefaultColliderInfo.EntityID = EntityID;
	rigidComp->DefaultColliderInfo.WorldLocation = rigidtransform.World_Location;
	rigidComp->DefaultColliderInfo.WorldQuaternion = rigidtransform.World_Quaternion;
	rigidComp->DefaultColliderInfo.WorldScale = rigidtransform.World_Scale;
	if (!rigidComp->IsDynamic)
	{
		CreateStatic(rigidComp);
	}
	else
	{
		CreateDynamic(rigidComp);
	}
}


void PhysicSystem::CreateStatic(RigidBodyComponent* staticBody)
{
	switch (staticBody->ColliderShape)
	{
	case VPPhysics::EColliderShape::BOX:
	{
		staticBody->BoxInfo.colliderInfo = staticBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateStaticBody(staticBody->BoxInfo, staticBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::CAPSULE:
	{
		staticBody->CapsuleInfo.colliderInfo = staticBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateStaticBody(staticBody->CapsuleInfo, staticBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::SPHERE:
	{
		staticBody->SphereInfo.colliderInfo = staticBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateStaticBody(staticBody->SphereInfo, staticBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::CONVEX:
	{
		if (!staticBody->HasComponent<MeshComponent>())
			return;
		auto meshcomp = staticBody->GetComponent<MeshComponent>();

		if (!m_PhysicsEngine->HasConvexMeshResource(meshcomp->FBX))
		{
			VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			convexMeshResourceInfo.FBXName = meshcomp->FBX;
			///TODO GetVertices wstring으로
			std::string temp(convexMeshResourceInfo.FBXName.begin(), convexMeshResourceInfo.FBXName.end());
			convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(temp);
			if (convexMeshResourceInfo.Vertexs.empty())
				return; // Return null if no vertices were extracted

			m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
		}
		staticBody->ConvexColliderInfo.FBXName = meshcomp->FBX;
		staticBody->ConvexColliderInfo.colliderInfo = staticBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateStaticBody(staticBody->ConvexColliderInfo, staticBody->ColliderType);

	}
	break;

	default:
		break;
	}
}
void PhysicSystem::CreateDynamic(RigidBodyComponent* dynamicBody)
{
	switch (dynamicBody->ColliderShape)
	{
	case VPPhysics::EColliderShape::BOX:
	{
		dynamicBody->BoxInfo.colliderInfo = dynamicBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateDynamicBody(dynamicBody->BoxInfo, dynamicBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::CAPSULE:
	{
		dynamicBody->CapsuleInfo.colliderInfo = dynamicBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateDynamicBody(dynamicBody->CapsuleInfo, dynamicBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::SPHERE:
	{
		dynamicBody->SphereInfo.colliderInfo = dynamicBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateDynamicBody(dynamicBody->SphereInfo, dynamicBody->ColliderType);
	}
	break;
	case VPPhysics::EColliderShape::CONVEX:
	{
		if (!dynamicBody->HasComponent<MeshComponent>())
			return;
		auto meshcomp = dynamicBody->GetComponent<MeshComponent>();

		///해당 메쉬에 대한 Convex가 존재하지 않는가?
		if (!m_PhysicsEngine->HasConvexMeshResource(meshcomp->FBX))
		{
			//VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			//convexMeshResourceInfo.FBXName = meshcomp->FBX;
			//convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(convexMeshResourceInfo.FBXName);
			//if (convexMeshResourceInfo.Vertexs.empty())
			//return;
			//m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
			///=======================삭제 예정=======================
			///TODO GetVertices wstring으로
			VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			convexMeshResourceInfo.FBXName = meshcomp->FBX;
			std::string temp(meshcomp->FBX.begin(), meshcomp->FBX.end());
			convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(temp);
			if (convexMeshResourceInfo.Vertexs.empty())
				return; // Return null if no vertices were extracted
			m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
			///=======================================================
		}
		dynamicBody->ConvexColliderInfo.FBXName = meshcomp->FBX;
		dynamicBody->ConvexColliderInfo.colliderInfo = dynamicBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateDynamicBody(dynamicBody->ConvexColliderInfo, dynamicBody->ColliderType);
	}
	break;

	default:
		break;
	}
}
void PhysicSystem::CreateCapsuleController(uint32_t EntityID)
{
	auto entity = GetSceneManager()->GetEntity(EntityID);
	if (!entity->HasComponent<ControllerComponent>())
		return;
	auto controllercomp = entity->GetComponent<ControllerComponent>();
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

#pragma endregion
