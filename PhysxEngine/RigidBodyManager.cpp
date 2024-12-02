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
#pragma region Create Body
void RigidBodyManager::CreateStaticBody(const BoxColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// 박스 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 박스 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	physx::PxBoxGeometry mesh(
		std::abs(info.Extent.x * info.colliderInfo.WorldScale.x),
		std::abs(info.Extent.y * info.colliderInfo.WorldScale.y),
		std::abs(info.Extent.z * info.colliderInfo.WorldScale.z)
	);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 정적 바디 설정
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
void RigidBodyManager::CreateStaticBody(const SphereColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// 구체 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 구체 콜라이더를 위한 Shape 생성 (최대 절대 스케일 사용)
	float maxScale = std::abs(info.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = info.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 정적 바디 설정
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
	{
		assert(false); // 실패 시 디버그 중단
	}
}
void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// 캡슐 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 캡슐 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	VPMath::Vector3 scale = info.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(info.HalfHeight * scale.y); // 절대 높이 설정
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = info.Radius * maxScale; // 반지름 설정
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// 정적 바디 설정
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
	{
		assert(false); // 실패 시 디버그 중단
	}
}
void RigidBodyManager::CreateStaticBody(const ConvexColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// 컨벡스 메쉬 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 컨벡스 메쉬 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(info.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(info.colliderInfo.WorldScale.x),
		std::abs(info.colliderInfo.WorldScale.y),
		std::abs(info.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 정적 바디 설정
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::BoxColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// 박스 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 박스 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	physx::PxBoxGeometry mesh(
		std::abs(info.Extent.x * info.colliderInfo.WorldScale.x),
		std::abs(info.Extent.y * info.colliderInfo.WorldScale.y),
		std::abs(info.Extent.z * info.colliderInfo.WorldScale.z)
	);

	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 동적 바디 설정
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, collidertype, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::SphereColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engininfo)
{
	// 구체 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 구체 콜라이더를 위한 Shape 생성 (최대 절대 스케일 사용)
	float maxScale = std::abs(info.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = info.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 동적 바디 설정
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engininfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// 캡슐 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 캡슐 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	VPMath::Vector3 scale = info.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(info.HalfHeight * scale.y); // 절대 높이 설정
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = info.Radius * maxScale; // 반지름 설정
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// 동적 바디 설정
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engineinfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::ConvexColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// 컨벡스 메쉬 콜라이더를 위한 Material 생성
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// 컨벡스 메쉬 콜라이더를 위한 Shape 생성 (절대 스케일 값 사용)
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(info.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(info.colliderInfo.WorldScale.x),
		std::abs(info.colliderInfo.WorldScale.y),
		std::abs(info.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// 동적 바디 설정
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engineinfo);

	// 리지드 바디 초기화
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // 맵에 리지드 바디 삽입
		AddBodyScene(rigidBody); // 씬에 리지드 바디 추가
		shape->release(); // Shape 해제
	}
	else
		assert(false); // 실패 시 디버그 중단
}
#pragma endregion
#pragma region Setting Body

// 정적 리지드 바디를 설정하는 함수 (BoxColliderInfo 사용)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정 (PhysicsLayer와 CollisionMatrix 기반)
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer); // 현재 물리 레이어
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)]; // 충돌 행렬
	shape->setSimulationFilterData(filterdata); // Shape에 필터 데이터 적용

	// StaticRigidBody 생성 및 리턴
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody; // 맵에 등록
	return staticBody;
}
// 정적 리지드 바디를 설정하는 함수 (SphereColliderInfo 사용)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody 생성 및 리턴
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}
// 정적 리지드 바디를 설정하는 함수 (ConvexColliderInfo 사용)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody 생성 및 리턴
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}
// 정적 리지드 바디를 설정하는 함수 (CapsuleColliderInfo 사용)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engininfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody 생성 및 리턴
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engininfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}

// 동적 리지드 바디를 설정하는 함수 (BoxColliderInfo 사용)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody 생성 및 리턴
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // 맵에 등록
	return dynamicBody;
}
// 동적 리지드 바디를 설정하는 함수 (SphereColliderInfo 사용)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody 생성 및 리턴
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // 맵에 등록
	return dynamicBody;
}
// 동적 리지드 바디를 설정하는 함수 (ConvexColliderInfo 사용)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody 생성 및 리턴
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // 맵에 등록
	return dynamicBody;
}
// 동적 리지드 바디를 설정하는 함수 (CapsuleColliderInfo 사용)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// 필터 데이터 설정
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody 생성 및 리턴
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // 맵에 등록
	return dynamicBody;
}

#pragma endregion

#pragma region Add_Release
// 리지드 바디를 씬에 추가하는 함수
void RigidBodyManager::AddBodyScene(std::shared_ptr<RigidBody> body)
{
	// 리지드 바디가 DynamicRigidBody인지 확인하고 씬에 추가
	if (IsDynamic(body->GetID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<DynamicRigidBody>(body)->GetPxDynamicRigid());
	// 리지드 바디가 StaticRigidBody인지 확인하고 씬에 추가
	else if (IsStatic(body->GetID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<StaticRigidBody>(body)->GetPxStaticRigid());
}
// 리지드 바디를 씬에서 제거하는 함수
void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID); // 엔티티 ID로 리지드 바디 검색
	if (it == m_RigidBodyMap.end())
		return; // 엔티티가 없으면 반환

	auto tempbody = it->second;

	// 리지드 바디가 Dynamic 또는 Static인지 확인
	if (!Reflection::IsSameType<DynamicRigidBody>(tempbody->GetTypeID()) &&
		!Reflection::IsSameType<StaticRigidBody>(tempbody->GetTypeID()))
		return;

	// 리지드 바디를 타입에 맞게 캐스팅
	auto dynamicBody = std::dynamic_pointer_cast<DynamicRigidBody>(tempbody);
	auto staticBody = std::dynamic_pointer_cast<StaticRigidBody>(tempbody);

	// DynamicRigidBody를 씬에서 제거
	if (dynamicBody && dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
	{
		m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
		m_RigidBodyMap.erase(it); // 맵에서 제거
	}
	// StaticRigidBody를 씬에서 제거
	else if (staticBody && staticBody->GetPxStaticRigid()->getScene() == m_Scene)
	{
		m_Scene->removeActor(*staticBody->GetPxStaticRigid());
		m_RigidBodyMap.erase(it); // 맵에서 제거
	}
}
#pragma endregion

#pragma region Convert_Static_Dynamic
// Dynamic 리지드 바디를 Static으로 변환하는 함수
void RigidBodyManager::ConvertToStatic(uint32_t EntityID)
{
	auto rigidbody = GetRigidBody(EntityID); // 리지드 바디 검색
	if (!rigidbody)
		return; // 없으면 반환
	if (IsStatic(EntityID))
		return; // 이미 Static이면 반환

	// Dynamic 리지드 바디를 씬에서 제거
	if (auto dynamicActor = std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody))
	{
		m_Scene->removeActor(*dynamicActor->GetPxDynamicRigid());
	}

	// 타입에 따라 적절한 Static 리지드 바디 생성
	if (rigidbody->m_Boxinfo)
		CreateStaticBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Sphereinfo)
		CreateStaticBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Capulseinfo)
		CreateStaticBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Convexinfo)
		CreateStaticBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
}
// Static 리지드 바디를 Dynamic으로 변환하는 함수
void RigidBodyManager::ConvertToDynamic(uint32_t EntityID)
{
	auto rigidbody = GetRigidBody(EntityID); // 리지드 바디 검색
	if (!rigidbody)
		return; // 없으면 반환
	if (IsDynamic(EntityID))
		return; // 이미 Dynamic이면 반환

	// Static 리지드 바디를 씬에서 제거
	if (auto staticActor = std::dynamic_pointer_cast<StaticRigidBody>(rigidbody))
		m_Scene->removeActor(*staticActor->GetPxStaticRigid());

	// 타입에 따라 적절한 Dynamic 리지드 바디 생성
	if (rigidbody->m_Boxinfo)
		CreateDynamicBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Sphereinfo)
		CreateDynamicBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Capulseinfo)
		CreateDynamicBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Convexinfo)
		CreateDynamicBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
}
// Dynamic 리지드 바디를 특정 PhysicsLayer로 Static으로 변환하는 함수
void RigidBodyManager::ConvertToStaticWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer)
{
	auto rigidbody = GetRigidBody(EntityID); // 리지드 바디 검색
	if (!rigidbody)
		return; // 없으면 반환
	if (IsStatic(EntityID))
		return; // 이미 Static이면 반환

	// Dynamic 리지드 바디를 씬에서 제거
	if (auto dynamicActor = std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody))
		m_Scene->removeActor(*dynamicActor->GetPxDynamicRigid());

	// 타입에 따라 적절한 PhysicsLayer를 설정하고 Static 리지드 바디 생성
	if (rigidbody->m_Boxinfo)
	{
		rigidbody->m_Boxinfo->colliderInfo.PhysicsLayer = layer;
		CreateStaticBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Sphereinfo)
	{
		rigidbody->m_Sphereinfo->colliderInfo.PhysicsLayer = layer;
		CreateStaticBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Capulseinfo)
	{
		rigidbody->m_Capulseinfo->colliderInfo.PhysicsLayer = layer;
		CreateStaticBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Convexinfo)
	{
		rigidbody->m_Convexinfo->colliderInfo.PhysicsLayer = layer;
		CreateStaticBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
}
// Static 리지드 바디를 특정 PhysicsLayer로 Dynamic으로 변환하는 함수
void RigidBodyManager::ConvertToDynamicWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer)
{
	auto rigidbody = GetRigidBody(EntityID); // 리지드 바디 검색
	if (!rigidbody)
		return; // 없으면 반환
	if (IsDynamic(EntityID))
		return; // 이미 Dynamic이면 반환


	// Static 리지드 바디를 씬에서 제거
	if (auto staticActor = std::dynamic_pointer_cast<StaticRigidBody>(rigidbody))
		m_Scene->removeActor(*staticActor->GetPxStaticRigid());

	// 타입에 따라 적절한 PhysicsLayer를 설정하고 Dynamic 리지드 바디 생성
	if (rigidbody->m_Boxinfo)
	{
		rigidbody->m_Boxinfo->colliderInfo.PhysicsLayer = layer;
		CreateDynamicBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Sphereinfo)
	{
		rigidbody->m_Sphereinfo->colliderInfo.PhysicsLayer = layer;
		CreateDynamicBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Capulseinfo)
	{
		rigidbody->m_Capulseinfo->colliderInfo.PhysicsLayer = layer;
		CreateDynamicBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
	else if (rigidbody->m_Convexinfo)
	{
		rigidbody->m_Convexinfo->colliderInfo.PhysicsLayer = layer;
		CreateDynamicBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	}
}
#pragma endregion


// 리지드 바디를 가져오는 함수
std::shared_ptr<RigidBody> RigidBodyManager::GetRigidBody(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID); // 엔티티 ID로 검색
	if (it == m_RigidBodyMap.end())
		return nullptr; // 없으면 nullptr 반환
	return it->second; // 리지드 바디 반환
}

bool RigidBodyManager::HasRigidBody(uint32_t entityID)
{
	return m_RigidBodyMap.count(entityID) > 0;
}
bool RigidBodyManager::IsDynamic(uint32_t EntityID)
{
	auto rigid = GetRigidBody(EntityID);
	if (!rigid)
		return false;
	if (Reflection::IsSameType<DynamicRigidBody>(rigid->GetTypeID()))
		return true;
	return false;
}
bool RigidBodyManager::IsStatic(uint32_t EntityID)
{
	auto rigid = GetRigidBody(EntityID);
	if (!rigid)
		return false;
	if (Reflection::IsSameType<StaticRigidBody>(rigid->GetTypeID()))
		return true;
	return false;
}

// 특정 물리 레이어에서 씬의 정점과 면을 추출하는 함수
void RigidBodyManager::ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!scene)
		return; // 씬이 유효하지 않으면 반환

	// 씬에서 Static Rigid Actor만 가져옴
	PxU32 actorCount = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);
	std::vector<PxRigidStatic*> actors(actorCount);
	scene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

	// 각 Static Actor를 순회
	for (PxRigidStatic* actor : actors)
	{
		if (!actor)
			continue;

		// Actor에서 Shapes를 가져옴
		PxU32 shapeCount = actor->getNbShapes();
		std::vector<PxShape*> shapes(shapeCount);
		actor->getShapes(shapes.data(), shapeCount);

		for (PxShape* shape : shapes)
		{
			// Simulation Shape이면서 Trigger Shape이 아닌 경우 처리
			if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
			{
				// 필터 데이터를 확인하여 레이어와 비교
				PxFilterData filterData = shape->getSimulationFilterData();
				if (filterData.word0 == static_cast<PxU32>(layer))
				{
					// 해당 Actor의 정점 및 면 데이터를 추출
					RigidBodyManager::ExtractVerticesAndFaces(actor, outVertices, outIndices);
					break; // 하나의 Shape만 처리하면 Actor 처리를 중단
				}
			}
		}
	}
}

// 특정 엔티티의 정점과 면을 추출하는 함수
void RigidBodyManager::ExtractEntityVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!HasRigidBody(entityID))
		return; // 엔티티에 리지드 바디가 없으면 반환

	if (IsStatic(entityID)) // Static Rigid Body인지 확인
	{
		auto staticBody = dynamic_cast<StaticRigidBody*>(GetRigidBody(entityID).get());
		physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
		if (!pxRigidStatic)
			return;
		ExtractVerticesAndFaces(pxRigidStatic, outVertices, outIndices);
	}
	else if (IsDynamic(entityID)) // Dynamic Rigid Body인지 확인
	{
		auto dynamicrigid = dynamic_cast<DynamicRigidBody*>(GetRigidBody(entityID).get());
		physx::PxRigidDynamic* pxRigidDynamic = dynamicrigid->GetPxDynamicRigid();
		if (!pxRigidDynamic)
			return;
		ExtractVerticesAndFaces(pxRigidDynamic, outVertices, outIndices);
	}
}

// Rigid Actor에서 정점과 면을 추출하는 함수
void RigidBodyManager::ExtractVerticesAndFaces(PxRigidActor* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!actor)
		return; // Actor가 유효하지 않으면 반환

	// Actor의 전역 변환(월드 좌표계)을 가져옴
	PxTransform globalPose = actor->getGlobalPose();

	// Actor에서 Shapes를 가져옴
	PxU32 shapeCount = actor->getNbShapes();
	std::vector<PxShape*> shapes(shapeCount);
	actor->getShapes(shapes.data(), shapeCount);

	for (PxShape* shape : shapes)
	{
		// Simulation Shape으로 표시된 것만 처리
		if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			// Shape의 지오메트리를 가져옴
			PxGeometryHolder geometryHolder = shape->getGeometry();

			if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH) // 컨벡스 메쉬 처리
			{
				// Convex Mesh Geometry 가져오기
				PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
				PxConvexMesh* convexMesh = convexGeometry.convexMesh;

				// Convex Mesh 스케일 가져오기
				PxMeshScale scale = convexGeometry.scale;

				// 정점 데이터 추출
				const PxVec3* meshVertices = convexMesh->getVertices();
				PxU32 vertexCount = convexMesh->getNbVertices();

				// 현재 정점 데이터의 시작 인덱스 저장
				int baseIndex = static_cast<int>(outVertices.size());

				// 정점을 변환하고 월드 좌표계로 변환 후 추가
				for (PxU32 i = 0; i < vertexCount; ++i)
				{
					PxVec3 scaledVertex = scale.transform(meshVertices[i]); // 스케일 적용
					PxVec3 worldVertex = globalPose.transform(scaledVertex); // 월드 변환
					outVertices.emplace_back(worldVertex.x, worldVertex.y, worldVertex.z); // 출력 버퍼에 추가
				}

				// 면 데이터 추출 (인덱스 버퍼 사용)
				const PxU8* indexBuffer = convexMesh->getIndexBuffer();
				PxU32 polygonCount = convexMesh->getNbPolygons();

				for (PxU32 i = 0; i < polygonCount; ++i)
				{
					PxHullPolygon polygon;
					convexMesh->getPolygonData(i, polygon);

					// 폴리곤의 정점 인덱스 처리
					if (polygon.mNbVerts == 3) // 삼각형일 경우 바로 처리
						for (PxU32 j = 0; j < 3; ++j)
						{
							PxU32 vertexIndex = indexBuffer[polygon.mIndexBase + j];
							outIndices.push_back(baseIndex + static_cast<int>(vertexIndex));
						}
					else if (polygon.mNbVerts > 3) // 다각형일 경우 삼각형화 처리
						for (PxU32 j = 2; j < polygon.mNbVerts; ++j)
						{
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase]));
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j - 1]));
							outIndices.push_back(baseIndex + static_cast<int>(indexBuffer[polygon.mIndexBase + j]));
						}
				}
			}
		}
	}
}

// Rigid Actor에서 로컬 정점을 추출하는 함수
void RigidBodyManager::ExtractLocalVerticesFromActor(physx::PxRigidActor* actor, std::vector<VPMath::Vector3>& localVertices)
{
	if (!actor)
		return; // Actor가 유효하지 않으면 반환
	// Actor에서 Shapes를 가져옴
	PxU32 shapeCount = actor->getNbShapes();
	std::vector<PxShape*> shapes(shapeCount);
	actor->getShapes(shapes.data(), shapeCount);
	for (PxShape* shape : shapes)
	{
		// Simulation Shape으로 표시된 것만 처리
		if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			PxGeometryHolder geometryHolder = shape->getGeometry();

			// Convex Mesh인지 확인
			if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH)
			{
				PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
				PxConvexMesh* convexMesh = convexGeometry.convexMesh;

				if (!convexMesh)
					continue;

				// 로컬 정점 데이터 추출
				const PxVec3* meshVertices = convexMesh->getVertices();
				PxU32 vertexCount = convexMesh->getNbVertices();

				for (PxU32 i = 0; i < vertexCount; ++i)
				{
					localVertices.emplace_back(meshVertices[i].x, meshVertices[i].y, meshVertices[i].z);
				}
			}
		}
	}
}

std::vector<VPMath::Vector3> RigidBodyManager::GetConVexMeshVertex(uint32_t entityID)
{
	std::vector<VPMath::Vector3> localVertices;

	if (!HasRigidBody(entityID))
		return localVertices;

	auto rigidBody = GetRigidBody(entityID);

	if (IsStatic(entityID))
	{
		// StaticRigidBody 처리
		auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get());
		if (!staticBody)
			return localVertices;

		physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
		if (!pxRigidStatic)
			return localVertices;

		// PxRigidStatic에서 정점 가져오기
		ExtractLocalVerticesFromActor(pxRigidStatic, localVertices);
	}
	else if (IsDynamic(entityID))
	{
		// DynamicRigidBody 처리
		auto dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody.get());
		if (!dynamicBody)
			return localVertices;

		physx::PxRigidDynamic* pxRigidDynamic = dynamicBody->GetPxDynamicRigid();
		if (!pxRigidDynamic)
			return localVertices;

		// PxRigidDynamic에서 정점 가져오기
		ExtractLocalVerticesFromActor(pxRigidDynamic, localVertices);
	}

	return localVertices;
}


void RigidBodyManager::SetGobalPose(uint32_t entityID, const VPMath::Vector3& P, const VPMath::Quaternion& Q)
{
	auto temp = GetRigidBody(entityID);
	if (!temp) return;

	PxVec3 tempPos = { P.x, P.y, P.z };
	physx::PxQuat tempQuat = physx::PxQuat(Q.x, Q.y, Q.z, Q.w);
	tempQuat.normalize();


	if (IsDynamic(entityID))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		dynamicBody->GetPxDynamicRigid()->setGlobalPose({ tempPos, tempQuat });
	}
	else if (IsStatic(entityID))
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
		return { 0, 0, 0 };

	if (IsDynamic(entityID))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		PxVec3 velocity = dynamicBody->GetPxDynamicRigid()->getLinearVelocity();
		return { velocity.x, velocity.y, velocity.z };
	}
	else if (IsStatic(entityID))
	{
		return { 0, 0, 0 };
	}
		return {};
}

void RigidBodyManager::AddVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V)
{
	auto temp = GetRigidBody(entityID);
	if (!temp)
	{
		assert(false);
		return;
	}
	if (IsDynamic(entityID))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		VPMath::Vector3 Dir = dir;
		float length = Dir.Length(); // 벡터의 크기를 확인
		// 벡터의 크기가 0이면 예외 처리
		if (length == 0.0f)
		{
			return;
		}
		Dir.Normalize();
		PxVec3 force = { Dir.x, Dir.y, Dir.z };
		force *= V;
		dynamicBody->GetPxDynamicRigid()->addForce(force, PxForceMode::eVELOCITY_CHANGE);
	}
}
void RigidBodyManager::SetVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V) {
	auto temp = GetRigidBody(entityID);
	if (!temp) {
		assert(false);
		return;
	}
	if (IsDynamic(entityID))
	{
		auto dynamicBody = static_cast<DynamicRigidBody*>(temp.get());
		VPMath::Vector3 Dir = dir;
		float length = Dir.Length(); // 벡터의 크기를 확인

		// 벡터의 크기가 0이거나 아주 작은 경우
		if (length < 1e-6f || V == 0.0f) {
			dynamicBody->GetPxDynamicRigid()->setLinearVelocity({0,0,0}); // 속도 0으로 설정
			return;
		}

		Dir.Normalize();
		PxVec3 force = { Dir.x, Dir.y, Dir.z };
		force *= V;
		dynamicBody->GetPxDynamicRigid()->setLinearVelocity(force);
	}
}

VPMath::Vector3 RigidBodyManager::GetGobalLocation(uint32_t entityID)
{
	auto temp = GetRigidBody(entityID);
	if (IsDynamic(entityID))
	{
		auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		return { pose.p.x, pose.p.y, pose.p.z };
	}
	else if (IsStatic(entityID))
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

	if (IsDynamic(entityID))
	{
		auto dynamicbody = static_cast<DynamicRigidBody*>(temp.get());
		auto pose = dynamicbody->GetPxDynamicRigid()->getGlobalPose();
		VPMath::Quaternion tempQuat = { pose.q.x, pose.q.y, pose.q.z, pose.q.w };
		tempQuat.Normalize(); // Normalize quaternion after getting it
		return tempQuat;
	}
	else if (IsStatic(entityID))
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
	auto key = static_cast<VPPhysics::USERDATA*>(Actor->userData);
	return key->entityID;
}

RaycastData RigidBodyManager::RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
	RaycastData raycastresult{};
	auto tempActor = FindActorByID(entityID);
	if (!tempActor)
		return {};
	physx::PxVec3 tempDir = { dir.x,dir.y,dir.z };
	tempDir.normalize();
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here
	if (tempDir.isZero())
		return raycastresult;

	bool find = m_Scene->raycast(
		tempActor->getGlobalPose().p,				// 시작점
		tempDir,									// 단위벡터
		max,										// 거리
		buf);										// PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) { return a.distance < b.distance; });
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
		PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
		PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴

		auto tempID = FindIDByActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음
		if (tempID == entityID)
			continue;
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
		{

			raycastresult ={ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d };
			return raycastresult;
		}
	}
	return raycastresult;
}
RaycastData RigidBodyManager::RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
	RaycastData raycastresult{};
	auto tempActor = FindActorByID(entityID);			// entityID를 통해 물리 액터를 찾아옴
	if (!tempActor)
		return {};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };	// 입력된 방향 벡터를 PhysX의 PxVec3로 변환
	tempDir.normalize();								// 방향 벡터를 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;						// 레이캐스트할 최대 거리를 설정
	const PxU32 bufferSize = 32;						// 레이캐스트 히트 결과를 저장할 버퍼의 크기 설정
	PxRaycastHit hitBuffer[bufferSize];					// 레이캐스트에 의해 접촉된 물체들의 정보를 저장할 버퍼
	PxRaycastBuffer buf(hitBuffer, bufferSize);			// 레이캐스트 결과를 저장할 버퍼 객체 생성

	if (tempDir.isZero())	// 방향 벡터가 0인 경우(정규화 실패), 0을 반환
		return raycastresult;

	// 레이캐스트 시작점 설정: 액터의 전역 위치에 오프셋을 더한 값
	PxVec3 raypos = PxVec3{ offset.x, offset.y, offset.z } + tempActor->getGlobalPose().p;

	// 레이캐스트 실행
	bool find = m_Scene->raycast(
		raypos,               // 레이캐스트 시작점
		tempDir,              // 레이캐스트 방향 (정규화된 단위 벡터)
		max,                  // 레이캐스트 최대 거리
		buf);                 // 레이캐스트 결과를 저장할 버퍼

	if (!find)  // 레이캐스트로 아무것도 찾지 못한 경우 0을 반환
		return raycastresult;
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
		{

			 raycastresult={ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d };
			return raycastresult;
		}
	}

	return 	 raycastresult;  // 트리거가 아닌 충돌체를 찾지 못한 경우 0을 반환
}
RaycastData RigidBodyManager::RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	RaycastData raycastresult{};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
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
		{

			raycastresult ={ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d };
			return raycastresult;
		}
	}

	return raycastresult;
}

RaycastData RigidBodyManager::RaycastActorAtPose_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	RaycastData raycastresult{};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// 히트된 엔티티들 중에서 트리거가 아닌 충돌체의 ID를 반환
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{

		auto tempID = FindIDByActor(buf.touches[i].actor);	// i번째 접촉된 액터(물리 객체)와 관련된 엔티티의 ID를 찾음

		PxVec3 p = buf.getTouch(i).position;				// i번째로 레이캐스트에 의해 접촉된 지점의 위치를 가져옴
		PxVec3 n = buf.getTouch(i).normal;					// i번째 접촉된 지점의 법선 벡터(표면의 방향)을 가져옴
		PxF32 d = buf.getTouch(i).distance;					// i번째 접촉된 지점까지의 거리를 가져옴
		if (tempID == entityID)  // 자신과 동일한 엔티티 ID는 무시하고 다음 반복으로 넘어감
			continue;
		// 트리거가 아닌 충돌체인 경우에만 엔티티 ID 반환
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
		{

			raycastresult={ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d };
			return raycastresult;
		}
	}

	return raycastresult;
}

RaycastData RigidBodyManager::RaycastActorAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	RaycastData temp{};

	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return temp;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return temp;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// 히트된 엔티티들 중에서 트리거가 아닌 충돌체의 ID를 반환
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		auto tempID = FindIDByActor(buf.touches[i].actor); // i번째 접촉된 액터와 관련된 엔티티 ID

		// entityIDs에 포함된 ID는 무시
		if (std::find(entityIDs.begin(), entityIDs.end(), tempID) != entityIDs.end())
			continue;

		// 트리거가 아닌 충돌체인 경우에만 처리
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)) {
			PxVec3 p = buf.getTouch(i).position;  // 접촉된 지점의 위치
			PxVec3 n = buf.getTouch(i).normal;    // 접촉된 지점의 법선 벡터
			PxF32 d = buf.getTouch(i).distance;   // 접촉된 지점까지의 거리
			temp = { tempID, {p.x, p.y, p.z}, {n.x, n.y, n.z}, d };
			return temp;
		}
	}
	return temp;
}

std::vector<RaycastData> RigidBodyManager::RaycastActorsAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	//RaycastData raycastresult{};
	std::vector<RaycastData> raycastresult{};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
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
		{
			raycastresult.push_back ({ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d });
		}
	}
	return raycastresult;
}

std::vector<RaycastData> RigidBodyManager::RaycastActorsAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	std::vector<RaycastData> raycastresult{};

	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // 단위 벡터로 정규화
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// 주어진 위치를 PhysX의 PxVec3로 변환
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// 레이캐스트 수행
	bool find = m_Scene->raycast(
		raypos,                                   // 시작점
		tempDir,                                  // 단위벡터
		max,                                      // 거리
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// 히트된 엔티티들 중에서 트리거가 아닌 충돌체의 ID를 반환
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		auto tempID = FindIDByActor(buf.touches[i].actor); // i번째 접촉된 액터와 관련된 엔티티 ID

		// entityIDs에 포함된 ID는 무시
		if (std::find(entityIDs.begin(), entityIDs.end(), tempID) != entityIDs.end())
			continue;

		// 트리거가 아닌 충돌체인 경우에만 처리
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)) {
			PxVec3 p = buf.getTouch(i).position;  // 접촉된 지점의 위치
			PxVec3 n = buf.getTouch(i).normal;    // 접촉된 지점의 법선 벡터
			PxF32 d = buf.getTouch(i).distance;   // 접촉된 지점까지의 거리
			raycastresult.push_back({ tempID, {p.x, p.y, p.z}, {n.x, n.y, n.z}, d }) ;
		}
	}
	return raycastresult;
}



physx::PxRigidActor* RigidBodyManager::FindActorByID(uint32_t entityID)
{
	// 먼저 m_RigidBodyMap에서 엔티티를 찾음
	auto it = m_RigidBodyMap.find(entityID);
	if (it == m_RigidBodyMap.end())
		return nullptr;

	auto rigidbody = it->second;  // 검색한 결과의 값을 가져옴
	// DynamicRigidBody 타입인지 먼저 체크 후 반환
	if (IsDynamic(entityID))
		return std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody)->GetPxDynamicRigid();

	// StaticRigidBody 타입인지 체크 후 반환
	if (IsStatic(entityID))
		return std::dynamic_pointer_cast<StaticRigidBody>(rigidbody)->GetPxStaticRigid();
	return nullptr;  // 해당하는 타입이 없으면 nullptr 반환
}

