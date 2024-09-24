#include "pch.h"
#include "RigidBodyManager.h"
#include "StaticRigidBody.h"
#include "EventManager.h"
#include "DynamicRigidBody.h"
#include "ConvexMeshResource.h"
#include "PhysichResourceManager.h"
#include <cmath> // For std::abs
#include <iostream>

RigidBodyManager::RigidBodyManager()
{
	//EventManager::GetInstance().Subscribe("OnAddBodyScene", CreateSubscriber(&RigidBodyManager::OnAddBodyScene));
	EventManager::GetInstance().Subscribe("OnReleaseBodyScene", CreateSubscriber(&RigidBodyManager::OnReleaseBodyScene));
}

RigidBodyManager::~RigidBodyManager()
{
	m_RigidBodyMap.clear(); // 확인해보기
}

bool RigidBodyManager::Initialize(physx::PxPhysics* physics, physx::PxScene* scene, std::shared_ptr<PhysichResourceManager> resourceManager)
{
	m_Physics = physics;
	m_Scene = scene;
	m_ResourceManager = resourceManager;
	return true;
} 

void RigidBodyManager::Update()
{
}

void RigidBodyManager::CreateStaticBody(const BoxColliderInfo& boxinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// Create the material for the box
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);

	// Create the shape for the box collider with absolute scale values
	physx::PxBoxGeometry mesh(
		std::abs(boxinfo.Extent.x * boxinfo.colliderInfo.WorldScale.x),
		std::abs(boxinfo.Extent.y * boxinfo.colliderInfo.WorldScale.y),
		std::abs(boxinfo.Extent.z * boxinfo.colliderInfo.WorldScale.z)
	);

	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the static body
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, boxinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
	{
		rigidBody->SetExtent(boxinfo.Extent);

		// Insert the rigid body into the map
		m_RigidBodyMap[boxinfo.colliderInfo.EntityID] = rigidBody;

		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateStaticBody(const SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// Create the material for the Sphere
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);

	// Create the shape for the sphere collider with the maximum absolute scale
	float maxScale = std::abs(sphereinfo.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = sphereinfo.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the static body
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
	{
		rigidBody->SetRadius(sphereinfo.Radius);

		// Insert the rigid body into the map
		m_RigidBodyMap[sphereinfo.colliderInfo.EntityID] = rigidBody;

		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// Create the material for the collider
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);

	// Create the shape for the capsule collider with absolute scale values
	VPMath::Vector3 scale = capsuleinfo.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(capsuleinfo.HalfHeight * scale.y);
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = capsuleinfo.Radius * maxScale;
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// Set up the static body
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
	{
		// Insert the rigid body into the map
		m_RigidBodyMap[capsuleinfo.colliderInfo.EntityID] = rigidBody;

		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// Create the material for the collider
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(convexMeshinfo.colliderInfo.StaticFriction, convexMeshinfo.colliderInfo.DynamicFriction, convexMeshinfo.colliderInfo.Restitution);

	// Create the shape for the Convex collider with absolute scale values
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(convexMeshinfo.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(convexMeshinfo.colliderInfo.WorldScale.x),
		std::abs(convexMeshinfo.colliderInfo.WorldScale.y),
		std::abs(convexMeshinfo.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the static body
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, convexMeshinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(convexMeshinfo.colliderInfo, shape, m_Physics))
	{
		// Insert the rigid body into the map
		m_RigidBodyMap[convexMeshinfo.colliderInfo.EntityID] = rigidBody;

		// Add the rigid body to the scene
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);

	// Create the shape for the box collider with absolute scale values
	physx::PxBoxGeometry mesh(
		std::abs(boxinfo.Extent.x * boxinfo.colliderInfo.WorldScale.x),
		std::abs(boxinfo.Extent.y * boxinfo.colliderInfo.WorldScale.y),
		std::abs(boxinfo.Extent.z * boxinfo.colliderInfo.WorldScale.z)
	);

	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the dynamic body
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, boxinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(boxinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap[boxinfo.colliderInfo.EntityID] = rigidBody;
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetExtent(boxinfo.Extent);
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(sphereinfo.colliderInfo.StaticFriction, sphereinfo.colliderInfo.DynamicFriction, sphereinfo.colliderInfo.Restitution);

	// Create the shape for the sphere collider with the maximum absolute scale
	float maxScale = std::abs(sphereinfo.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = sphereinfo.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the dynamic body
	std::shared_ptr<DynamicRigidBody>  rigidBody = SettingDynamicBody(shape, sphereinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(sphereinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap[sphereinfo.colliderInfo.EntityID] = rigidBody;
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetRadius(sphereinfo.Radius);
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(capsuleinfo.colliderInfo.StaticFriction, capsuleinfo.colliderInfo.DynamicFriction, capsuleinfo.colliderInfo.Restitution);

	// Create the shape for the capsule collider with absolute scale values
	VPMath::Vector3 scale = capsuleinfo.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(capsuleinfo.HalfHeight * scale.y);
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = capsuleinfo.Radius * maxScale;
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// Set up the dynamic body
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, capsuleinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(capsuleinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap[capsuleinfo.colliderInfo.EntityID] = rigidBody;
		AddBodyScene(rigidBody);
		shape->release();
		rigidBody->SetHalfHeight(capsuleinfo.HalfHeight);
		rigidBody->SetRadius(capsuleinfo.Radius);
	}
	else
	{
		assert(false);
	}
}

void RigidBodyManager::CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(convexMeshinfo.colliderInfo.StaticFriction, convexMeshinfo.colliderInfo.DynamicFriction, convexMeshinfo.colliderInfo.Restitution);

	// Create the shape for the Convex collider with absolute scale values
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(convexMeshinfo.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(convexMeshinfo.colliderInfo.WorldScale.x),
		std::abs(convexMeshinfo.colliderInfo.WorldScale.y),
		std::abs(convexMeshinfo.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// Set up the dynamic body
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, convexMeshinfo.colliderInfo, collidertype, engininfo);

	// Initialize the rigid body
	if (rigidBody->Initialize(convexMeshinfo.colliderInfo, shape, m_Physics))
	{
		m_RigidBodyMap[convexMeshinfo.colliderInfo.EntityID] = rigidBody;
		AddBodyScene(rigidBody);
		shape->release();
	}
	else
	{
		assert(false);
	}
}

std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.PhysicsLayer);
	filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	auto staticBody = std::make_shared<StaticRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}

std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.PhysicsLayer);
	filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	auto dynamicBody = std::make_shared<DynamicRigidBody>(colliderType, info.EntityID, info.PhysicsLayer);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody;
	return dynamicBody;
}

void RigidBodyManager::OnAddBodyScene(std::shared_ptr<RigidBody> rigidbody)
{
	if (Reflection::IsSameType<DynamicRigidBody>(rigidbody->GetTypeID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody)->GetPxDynamicRigid());
	else if (Reflection::IsSameType<StaticRigidBody>(rigidbody->GetTypeID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<StaticRigidBody>(rigidbody)->GetPxStaticRigid());

}
void RigidBodyManager::OnReleaseBodyScene(std::any data)
{
	auto [rigidBody, isDynamic] = std::any_cast<std::pair<std::shared_ptr<RigidBody>, bool>>(data);

	if (isDynamic)
	{
		if (auto dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody.get()))
		{
			m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
		}
	}
	else
	{
		if (auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get()))
		{
			m_Scene->removeActor(*staticBody->GetPxStaticRigid());
		}
	}
}

void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID);
	if (it == m_RigidBodyMap.end())
		return;

	auto tempbody = it->second;
	bool isDynamic = false;

	// DynamicRigidBody인지 체크 후 처리
	if (Reflection::IsSameType<DynamicRigidBody>(tempbody->GetTypeID()))
	{
		auto dynamicBody = std::static_pointer_cast<DynamicRigidBody>(tempbody);
		if (dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
		{
			m_RigidBodyMap.erase(it);
			isDynamic = true;
		}
	}
	// StaticRigidBody인지 체크 후 처리
	else if (Reflection::IsSameType<StaticRigidBody>(tempbody->GetTypeID()))
	{
		auto staticBody = std::static_pointer_cast<StaticRigidBody>(tempbody);
		if (staticBody->GetPxStaticRigid()->getScene() == m_Scene)
		{
			m_RigidBodyMap.erase(it);
		}
	}

	// tempbody와 isDynamic의 값을 std::any로 래핑하여 이벤트를 호출
	std::any data = std::make_pair(tempbody, isDynamic);
	OnReleaseBodyScene(data);
}


std::shared_ptr<RigidBody> RigidBodyManager::GetRigidBody(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID);
	if (it == m_RigidBodyMap.end())
		return nullptr;
	return it->second;
}

bool RigidBodyManager::HasRigidBody(uint32_t entityID)
{
	return m_RigidBodyMap.count(entityID) > 0;
}

void RigidBodyManager::ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!scene)
		return;

	// Retrieve only static rigid actors from the scene
	PxU32 actorCount = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);
	std::vector<PxRigidStatic*> actors(actorCount);
	scene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

	// Iterate over all static actors
	for (PxRigidStatic* actor : actors)
	{
		if (!actor)
			continue;

		// Retrieve shapes from the PxRigidStatic actor
		PxU32 shapeCount = actor->getNbShapes();
		std::vector<PxShape*> shapes(shapeCount);
		actor->getShapes(shapes.data(), shapeCount);

		for (PxShape* shape : shapes)
		{
			// Check if the shape is a simulation shape and not a trigger shape
			if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
			{
				// Check the shape's filter data for the layer
				PxFilterData filterData = shape->getSimulationFilterData();
				if (filterData.word0 == static_cast<PxU32>(layer))
				{
					// Use the existing function to extract vertices and faces for this actor
					RigidBodyManager::ExtractVerticesAndFaces(actor, outVertices, outIndices);
					break; // No need to check other shapes if we've processed the actor
				}
			}
		}
	}
}

void RigidBodyManager::ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!HasRigidBody(entityID))
		return;

	auto rigidBody = GetRigidBody(entityID);
	auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get());
	if (!staticBody)
		return;

	physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
	if (!pxRigidStatic)
		return;

	// Use the previously implemented function to extract vertices and faces
	ExtractVerticesAndFaces(pxRigidStatic, outVertices, outIndices);
}
void RigidBodyManager::ExtractVerticesAndFaces(PxRigidStatic* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!actor)
		return;

	// 액터의 전역 포즈(월드 변환)를 가져옵니다.
	PxTransform globalPose = actor->getGlobalPose();

	// PxRigidStatic 액터에서 쉐이프들을 가져옵니다.
	PxU32 shapeCount = actor->getNbShapes();
	std::vector<PxShape*> shapes(shapeCount);
	actor->getShapes(shapes.data(), shapeCount);

	for (PxShape* shape : shapes)
	{
		// SIMULATION_SHAPE(충돌 용도)로 표시된 쉐이프만 처리합니다.
		if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			// 쉐이프에서 지오메트리를 가져옵니다.
			PxGeometryHolder geometryHolder = shape->getGeometry();

			if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH)
			{
				// 컨벡스 메쉬 지오메트리 접근
				PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
				PxConvexMesh* convexMesh = convexGeometry.convexMesh;

				// convexGeometry에서 스케일을 추출합니다.
				PxMeshScale scale = convexGeometry.scale;

				// PxConvexMesh에서 정점들을 추출합니다.
				const PxVec3* meshVertices = convexMesh->getVertices();
				PxU32 vertexCount = convexMesh->getNbVertices();

				// 이 정점 배치의 시작 인덱스를 저장합니다.
				int baseIndex = static_cast<int>(outVertices.size());

				// 정점을 변환하여 월드 공간으로 변환한 후 outVertices에 추가합니다.
				for (PxU32 i = 0; i < vertexCount; ++i)
				{
					// 먼저 로컬 정점에 스케일을 적용합니다.
					PxVec3 scaledVertex = scale.transform(meshVertices[i]);

					// 스케일링된 정점을 액터의 전역 포즈를 사용하여 월드 공간으로 변환합니다.
					PxVec3 worldVertex = globalPose.transform(scaledVertex);

					// 변환된 정점을 출력 벡터에 추가합니다.
					outVertices.emplace_back(worldVertex.x, worldVertex.y, worldVertex.z);
				}

				// 인덱스를 추출합니다 (인덱스 버퍼는 정점 버퍼를 참조합니다).
				const PxU8* indexBuffer = convexMesh->getIndexBuffer();
				PxU32 polygonCount = convexMesh->getNbPolygons();

				for (PxU32 i = 0; i < polygonCount; ++i)
				{
					PxHullPolygon polygon;
					convexMesh->getPolygonData(i, polygon);

					// 폴리곤의 정점 인덱스를 추출합니다.
					if (polygon.mNbVerts == 3) // 이미 삼각형
					{
						for (PxU32 j = 0; j < 3; ++j)
						{
							PxU32 vertexIndex = indexBuffer[polygon.mIndexBase + j];
							outIndices.push_back(baseIndex + static_cast<int>(vertexIndex));
						}
					}
					else if (polygon.mNbVerts > 3) // 3개 이상의 정점을 가진 폴리곤(삼각형화가 필요)
					{
						for (PxU32 j = 2; j < polygon.mNbVerts; ++j)
						{
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase]));
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j - 1]));
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j]));
						}
					}

					// 디버깅 출력
					std::cout << "폴리곤 " << i << " 은(는) " << polygon.mNbVerts << " 개의 정점을 가지고 있습니다.\n";
					std::cout << "현재까지의 인덱스 개수: " << outIndices.size() << "\n";
				}
			}
		}
	}
}

void RigidBodyManager::SetGobalPose(uint32_t entityID, const VPMath::Vector3& P, const VPMath::Quaternion& Q)
{
	auto temp = GetRigidBody(entityID);
	if (!temp) return;

	PxVec3 tempPos = { P.x, P.y, P.z };
	physx::PxQuat tempQuat = physx::PxQuat(Q.x, Q.y, Q.z, Q.w);
	tempQuat.normalize();


	if (Reflection::IsSameType<DynamicRigidBody>(temp->GetTypeID()))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		dynamicBody->GetPxDynamicRigid()->setGlobalPose({ tempPos, tempQuat });
	}
	else if (Reflection::IsSameType<StaticRigidBody>(temp->GetTypeID()))
	{
		auto staticBody = static_cast<StaticRigidBody*>(temp.get());
		staticBody->GetPxStaticRigid()->setGlobalPose({ tempPos, tempQuat });
	}
	else
	{
		assert(false);
	}
}

VPMath::Vector3 RigidBodyManager::GetVelocity(uint32_t entityID)
{
	auto temp = GetRigidBody(entityID);
	if (!temp)
	{
		assert(false);
	}
	if (Reflection::IsSameType<DynamicRigidBody>(temp->GetTypeID()))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		PxVec3 velocity = dynamicBody->GetPxDynamicRigid()->getLinearVelocity();
		return { velocity.x, velocity.y, velocity.z };
	}
	else if (Reflection::IsSameType<StaticRigidBody>(temp->GetTypeID()))
	{
		return { 0, 0, 0 };
	}
	else
	{
		assert(false);
		return {};
	}
}

void RigidBodyManager::AddVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V)
{
	auto temp = GetRigidBody(entityID);
	if (!temp)
	{
		assert(false);
		return;
	}
	if (Reflection::IsSameType<DynamicRigidBody>(temp->GetTypeID()))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		VPMath::Vector3 Dir = dir;
		Dir.Normalize();
		PxVec3 force = { Dir.x, Dir.y, Dir.z };
		force *= V;
		dynamicBody->GetPxDynamicRigid()->addForce(force, PxForceMode::eACCELERATION);
	}
}

VPMath::Vector3 RigidBodyManager::GetGobalLocation(uint32_t entityID)
{
	auto temp = GetRigidBody(entityID);
	if (Reflection::IsSameType<DynamicRigidBody>(temp->GetTypeID()))
	{
		auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		return { pose.p.x, pose.p.y, pose.p.z };
	}
	else if (Reflection::IsSameType<StaticRigidBody>(temp->GetTypeID()))
	{
		auto staticbody = static_cast<StaticRigidBody*>(temp.get());
		auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
		return { pose.p.x, pose.p.y, pose.p.z };
	}
	else
	{
		assert(false); // Add an assert to handle the unexpected case
		return {};
	}
}

VPMath::Quaternion RigidBodyManager::GetGobalQuaternion(uint32_t entityID)
{
	auto temp = GetRigidBody(entityID);

	if (Reflection::GetTypeID<DynamicRigidBody>() == temp->GetTypeID())
	{
		auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		VPMath::Quaternion tempQuat = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };
		tempQuat.Normalize(); // Normalize quaternion after getting it
		return tempQuat;
	}
	else if (Reflection::GetTypeID<StaticRigidBody>() == temp->GetTypeID())
	{
		auto staticbody = static_cast<StaticRigidBody*>(temp.get());
		auto pose = staticbody->GetPxStaticRigid()->getGlobalPose();
		VPMath::Quaternion tempQuat = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };
		tempQuat.Normalize(); // Normalize quaternion after getting it
		return tempQuat;
	}
	else
	{
		assert(false); // Add an assert to handle the unexpected case
		return {};
	}
}

uint32_t RigidBodyManager::FindIDByActor(physx::PxRigidActor* Actor)
{
	auto key = static_cast<uint32_t*>(Actor->userData);
	return *key;
}

uint32_t RigidBodyManager::RaycastToHitActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
	auto tempActor = FindActorByID(entityID);
	physx::PxVec3 tempDir = { dir.x,dir.y,dir.z };
	tempDir.normalize();
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here
	if (tempDir.isZero())
		return 0;

	bool find = m_Scene->raycast(
		tempActor->getGlobalPose().p,				// 시작점
		tempDir,									// 단위벡터
		max,										// 거리
		buf);										// PxRaycastCallback& hitCall

	if (!find)
		return 0;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{

		PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
		PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
		PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴
		auto tempID = FindIDByActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음
		if (tempID == entityID)
			continue;
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
			return tempID;
	}
	return 0;
}

uint32_t RigidBodyManager::RaycastToHitActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
	auto tempActor = FindActorByID(entityID);			// entityID를 통해 물리 액터를 찾아옴
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };	// 입력된 방향 벡터를 PhysX의 PxVec3로 변환
	tempDir.normalize();								// 방향 벡터를 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;						// 레이캐스트할 최대 거리를 설정
	const PxU32 bufferSize = 32;						// 레이캐스트 히트 결과를 저장할 버퍼의 크기 설정
	PxRaycastHit hitBuffer[bufferSize];					// 레이캐스트에 의해 접촉된 물체들의 정보를 저장할 버퍼
	PxRaycastBuffer buf(hitBuffer, bufferSize);			// 레이캐스트 결과를 저장할 버퍼 객체 생성

	if (tempDir.isZero())	// 방향 벡터가 0인 경우(정규화 실패), 0을 반환
		return 0;

	// 레이캐스트 시작점 설정: 액터의 전역 위치에 오프셋을 더한 값
	PxVec3 raypos = PxVec3{ offset.x, offset.y, offset.z } + tempActor->getGlobalPose().p;

	// 레이캐스트 실행
	bool find = m_Scene->raycast(
		raypos,               // 레이캐스트 시작점
		tempDir,              // 레이캐스트 방향 (정규화된 단위 벡터)
		max,                  // 레이캐스트 최대 거리
		buf);                 // 레이캐스트 결과를 저장할 버퍼

	if (!find)  // 레이캐스트로 아무것도 찾지 못한 경우 0을 반환
		return 0;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// 레이캐스트로 접촉된 모든 객체들에 대해 반복
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;   // i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
		PxVec3 n = buf.getTouch(i).normal;     // i번째 접촉된 지점의 표면 법선 벡터를 가져옴
		PxF32 d = buf.getTouch(i).distance;    // i번째 접촉된 지점까지의 거리를 가져옴
		auto tempID = FindIDByActor(buf.touches[i].actor);  // i번째 접촉된 액터(물리 객체)에 해당하는 엔티티 ID를 찾음

		if (tempID == entityID)  // 자신과 동일한 엔티티 ID는 무시하고 다음 반복으로 넘어감
			continue;

		// 트리거가 아닌 충돌체에 대해서만 해당 엔티티 ID를 반환
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
			return tempID;
	}

	return 0;  // 트리거가 아닌 충돌체를 찾지 못한 경우 0을 반환
}


uint32_t RigidBodyManager::RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return 0;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return 0;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// 히트된 엔티티들 중에서 트리거가 아닌 충돌체의 ID를 반환
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
		PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
		PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴
		auto tempID = FindIDByActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음

		// 트리거가 아닌 충돌체인 경우에만 엔티티 ID 반환
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
			return tempID;
	}

	return 0;
}



physx::PxRigidActor* RigidBodyManager::FindActorByID(uint32_t entityID)
{
	// 먼저 m_RigidBodyMap에서 엔티티를 찾음
	auto it = m_RigidBodyMap.find(entityID);
	if (it == m_RigidBodyMap.end())
		return nullptr;

	auto rigidbody = it->second;  // 검색한 결과의 값을 가져옴
	Reflection::IsSameType<DynamicRigidBody>(rigidbody->GetTypeID());
	// DynamicRigidBody 타입인지 먼저 체크 후 반환
	if (Reflection::IsSameType<DynamicRigidBody>(rigidbody->GetTypeID()))
		return std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody)->GetPxDynamicRigid();

	// StaticRigidBody 타입인지 체크 후 반환
	if (Reflection::IsSameType<StaticRigidBody>(rigidbody->GetTypeID()))
		return std::dynamic_pointer_cast<StaticRigidBody>(rigidbody)->GetPxStaticRigid();

	return nullptr;  // 해당하는 타입이 없으면 nullptr 반환
}

void RigidBodyManager::AddBodyScene(std::shared_ptr<RigidBody> body)
{
	//EventManager::GetInstance().ScheduleEvent("OnAddBodyScene", body);
	OnAddBodyScene(body);
}
