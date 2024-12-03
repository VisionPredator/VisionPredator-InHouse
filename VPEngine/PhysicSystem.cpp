#include "pch.h"
#include "PhysicSystem.h"
#include "../PhysxEngine/IPhysx.h"
#include "TransformSystem.h"
#include "EventManager.h"
#include "../VPGraphics/Log.h"

PhysicSystem::PhysicSystem(std::shared_ptr<SceneManager> sceneManager)
	:System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnAddVelecity", CreateSubscriber(&PhysicSystem::OnAddVelecity));
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
	// "OnUpdate" 이벤트를 즉시 발생시킴
	EventManager::GetInstance().ImmediateEvent("OnUpdate");

	// RigidBodyComponent 처리
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		uint32_t entityID = rigidBodyComponent.GetEntityID();
		// 해당 엔티티의 RigidBody가 없으면 건너뜀
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;
		// TransformComponent 가져오기
		auto rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		// 현재 위치와 회전을 가져옴
		auto templocation = m_PhysicsEngine->GetGobalLocation(entityID);
		float offset_T = (rigidBodyTransform->World_Location - templocation).Length();
		VPMath::Quaternion tempQuat = m_PhysicsEngine->GetGobalQuaternion(entityID);
		float offset_R = (rigidBodyTransform->World_Quaternion - tempQuat).Length();
		// 위치와 회전 차이가 임계값을 넘으면 물리 엔진에 갱신
		if (offset_R > m_rotation_threshold_degrees || offset_T > m_location_threshold)
			m_PhysicsEngine->SetGobalPose(entityID, rigidBodyTransform->World_Location, rigidBodyTransform->World_Quaternion);
		// 속도 업데이트
		rigidBodyComponent.Speed = m_PhysicsEngine->GetVelocity(entityID);
	}

	// ControllerComponent 처리
	for (ControllerComponent& controllerCompoent : COMPITER(ControllerComponent))
	{
		uint32_t entityID = controllerCompoent.GetEntityID();

		// 해당 엔티티의 Controller가 없으면 건너뜀
		if (!m_PhysicsEngine->HasController(entityID))
			continue;
		// TransformComponent 가져오기
		TransformComponent* controllerTransform = controllerCompoent.GetComponent<TransformComponent>();
		// 현재 위치를 가져와 Pivot과 Offset 제거
		VPMath::Vector3 templocation = m_PhysicsEngine->GetControllerGobalPose(entityID);
		templocation = DisApplyPivotAndOffset(controllerCompoent, templocation);
		// 위치 차이가 임계값을 넘으면 물리 엔진에 갱신
		if ((controllerTransform->World_Location - templocation).Length() > m_location_threshold)
		{
			templocation = ApplyPivotAndOffset(controllerCompoent, controllerTransform->World_Location);
			m_PhysicsEngine->SetControllerGobalPose(entityID, templocation);
		}
	}

	// 물리 엔진 업데이트
	m_PhysicsEngine->Update(deltaTime);

	// 동적인 RigidBodyComponent를 처리하여 Transform 업데이트
	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		uint32_t entityID = rigidBodyComponent.GetEntityID();

		// 해당 엔티티의 RigidBody가 없으면 건너뜀
		if (!m_PhysicsEngine->HasRigidBody(entityID))
			continue;

		// TransformComponent 가져오기
		auto rigidBodyTransform = rigidBodyComponent.GetComponent<TransformComponent>();

		// 동적 RigidBody가 아니면 건너뜀
		if (!m_PhysicsEngine->IsDynamic(entityID))
			continue;

		// 속도가 일정 이하이면 건너뜀
		rigidBodyComponent.Speed = m_PhysicsEngine->GetVelocity(entityID);
		if (rigidBodyComponent.Speed.Length() < 0.001f)
			continue;
		// TransformComponent의 위치와 회전 업데이트
		rigidBodyTransform->SetWorldLocation(m_PhysicsEngine->GetGobalLocation(entityID));
		rigidBodyTransform->SetWorldQuaternion(m_PhysicsEngine->GetGobalQuaternion(entityID));
	}

	// ControllerComponent를 처리하여 Transform 업데이트
	for (ControllerComponent& controllerComponent : COMPITER(ControllerComponent))
	{
		uint32_t entityID = controllerComponent.GetEntityID();
		// 해당 엔티티의 Controller가 없으면 건너뜀
		if (!m_PhysicsEngine->HasController(entityID))
			continue;
		// TransformComponent 가져오기
		auto controllerTransform = controllerComponent.GetComponent<TransformComponent>();
		// 현재 위치를 가져와 Pivot과 Offset 제거 후 Transform 업데이트
		auto templocation = m_PhysicsEngine->GetControllerGobalPose(entityID);
		templocation = DisApplyPivotAndOffset(controllerComponent, templocation);
		controllerTransform->SetWorldLocation(templocation);
		// TransformSystem에 업데이트 데이터 추가
		TransformSystem::AddUpdateData(controllerTransform);
	}

	// "OnUpdate" 이벤트를 다시 즉시 발생시킴
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


// 쿼터니언의 켤레를 계산하는 함수
VPMath::Quaternion QuaternionConjugate(const VPMath::Quaternion& q)
{
	// x, y, z는 부호를 반전하고 w는 그대로 유지
	return VPMath::Quaternion(-q.x, -q.y, -q.z, q.w);
}

// 두 쿼터니언 간의 각도 차이를 계산하는 함수
float QuaternionAngleDifference(const VPMath::Quaternion& q1, const VPMath::Quaternion& q2)
{
	// 두 쿼터니언의 델타를 계산
	VPMath::Quaternion delta = q1 * QuaternionConjugate(q2);
	delta.Normalize(); // 델타 쿼터니언을 정규화

	// w 컴포넌트를 -1.0과 1.0 사이로 제한
	float angleInRadians = 2.0f * std::acos(std::clamp(delta.w, -1.0f, 1.0f));

	// 라디안 값을 각도로 변환하여 반환
	return angleInRadians * (180.0f / VPMath::XM_PI);
}

#pragma endregion
#pragma region Render
void PhysicSystem::BeginRenderUpdate(float deltaTime)
{
	if (!m_Graphics->GetDebugRenderSetting())
		return;

	debug::OBBInfo obbInfo{};
	debug::SphereInfo sphereInfo{};

	for (RigidBodyComponent& rigidBodyComponent : COMPITER(RigidBodyComponent))
	{
		if (m_PhysicsEngine->IsEntityInCollision(rigidBodyComponent.GetEntityID()))
			rigidBodyComponent.DebugLineColor = { 1,0,0,1 };
		else if (m_PhysicsEngine->IsEntityInTrigger(rigidBodyComponent.GetEntityID()))
			rigidBodyComponent.DebugLineColor = { 0,1,0,1 };
		else
			rigidBodyComponent.DebugLineColor = { 1,1,1,1 };
		
		auto rigidTransform = rigidBodyComponent.GetComponent<TransformComponent>();
		debug::RayInfo frontInfo{};
		frontInfo.Color = { 0,0,1,1 };
		frontInfo.Origin = rigidTransform->World_Location;
		frontInfo.Direction = 10 * rigidTransform->FrontVector;
		frontInfo.Normalize = false;

		switch (rigidBodyComponent.ColliderShape)
		{
		case VPPhysics::EColliderShape::BOX:
		{


			obbInfo.OBB.Center = rigidTransform->World_Location;
			obbInfo.OBB.Extents = rigidBodyComponent.BoxInfo.Extent * rigidTransform->World_Scale;
			obbInfo.xAxisAngle = rigidTransform->World_Rotation.x;
			obbInfo.Color = rigidBodyComponent.DebugLineColor;
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
			sphereInfo.Color = rigidBodyComponent.DebugLineColor;

			m_Graphics->DrawSphere(sphereInfo);

		}
		break;
		case VPPhysics::EColliderShape::CONVEX:
		{
			// Convex Mesh의 로컬 정점을 가져옵니다.
			if (rigidBodyComponent.cachedWorldVertices.empty())
				return;

			auto transform = rigidBodyComponent.GetComponent<TransformComponent>();

			// 이미 존재하는 WorldTransform 사용
			const auto& worldTransform = transform->WorldTransform;

			// 변환된 월드 정점 계산
			std::vector<VPMath::Vector3> worldVertices;
			for (const auto& vertex : rigidBodyComponent.cachedWorldVertices)
				worldVertices.push_back(VPMath::Vector3::Transform(vertex, worldTransform));

			// Ray의 색상 설정 (동일한 색상 사용)
			VPMath::Vector4 rayColor = { 1, 0, 0, 1 }; // Red color for all rays

			// 디버그 데이터 렌더링
			for (size_t i = 0; i < worldVertices.size() - 1; ++i)
			{
				VPMath::Vector3 currentVertex = worldVertices[i];
				VPMath::Vector3 nextVertex = worldVertices[i + 1];

				// 방향 벡터 계산
				VPMath::Vector3 direction = nextVertex - currentVertex;

				// RayInfo 생성
				debug::RayInfo rayInfo{};
				rayInfo.Color = rigidBodyComponent.DebugLineColor;
				rayInfo.Origin = currentVertex;
				rayInfo.Direction = direction;
				rayInfo.Normalize = false;

				// Ray를 그립니다.
				m_Graphics->DrawRay(rayInfo);
			}

			// 마지막 정점과 첫 번째 정점 연결
			if (!worldVertices.empty())
			{
				VPMath::Vector3 lastVertex = worldVertices.back();
				VPMath::Vector3 firstVertex = worldVertices.front();

				VPMath::Vector3 direction = firstVertex - lastVertex;

				debug::RayInfo rayInfo{};
				rayInfo.Color = rigidBodyComponent.DebugLineColor; // 동일한 색상
				rayInfo.Origin = lastVertex;
				rayInfo.Direction = direction;
				rayInfo.Normalize = false;

				m_Graphics->DrawRay(rayInfo);
			}
		}
		break;
		default:
			break;
		}

	}
	for (ControllerComponent& ControllerComp : COMPITER(ControllerComponent))
	{
		if (m_PhysicsEngine->IsEntityInCollision(ControllerComp.GetEntityID()))
			ControllerComp.DebugLineColor = { 1,0,0,1 };
		else if (m_PhysicsEngine->IsEntityInTrigger(ControllerComp.GetEntityID()))
			ControllerComp.DebugLineColor = { 0,1,0,1 };
		else
			ControllerComp.DebugLineColor = { 1,1,1,1 };

		if (m_PhysicsEngine->IsEntityInCollision(ControllerComp.GetEntityID()))
			ControllerComp.DebugLineColor = { 1,0,0,1 };
		else if (m_PhysicsEngine->IsEntityInTrigger(ControllerComp.GetEntityID()))
			ControllerComp.DebugLineColor = { 0,1,0,1 };
		else
			ControllerComp.DebugLineColor = { 1,1,1,1 };

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
		obbInfo.Color = ControllerComp.DebugLineColor;
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
		m_Graphics->DrawRay(rightinfo);
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
void PhysicSystem::OnAddVelecity(std::any ID_Vector3_power)
{
	auto [entityid, dir, power] = std::any_cast<std::tuple<uint32_t, VPMath::Vector3,float>>(ID_Vector3_power);
	m_PhysicsEngine->AddVelocity(entityid, dir, power);
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
			convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(meshcomp->FBX);
			if (convexMeshResourceInfo.Vertexs.empty())
				return;

			m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
		}
		staticBody->ConvexColliderInfo.FBXName = meshcomp->FBX;
		staticBody->ConvexColliderInfo.colliderInfo = staticBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateStaticBody(staticBody->ConvexColliderInfo, staticBody->ColliderType);
		staticBody->cachedWorldVertices = m_PhysicsEngine->GetConVexMeshVertex(staticBody->GetEntityID());
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
			VPPhysics::ConvexMeshResourceInfo convexMeshResourceInfo;
			convexMeshResourceInfo.FBXName = meshcomp->FBX;
			convexMeshResourceInfo.Vertexs = m_Graphics->GetVertices(meshcomp->FBX);
			if (convexMeshResourceInfo.Vertexs.empty())
				return;
			m_PhysicsEngine->LoadConvexMeshResource(convexMeshResourceInfo);
		}
		dynamicBody->ConvexColliderInfo.FBXName = meshcomp->FBX;
		dynamicBody->ConvexColliderInfo.colliderInfo = dynamicBody->DefaultColliderInfo;
		m_PhysicsEngine->CreateDynamicBody(dynamicBody->ConvexColliderInfo, dynamicBody->ColliderType);
		dynamicBody->cachedWorldVertices = m_PhysicsEngine->GetConVexMeshVertex(dynamicBody->GetEntityID());

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
