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
	m_RigidBodyMap.clear(); // Ȯ���غ���
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
	// �ڽ� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// �ڽ� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	physx::PxBoxGeometry mesh(
		std::abs(info.Extent.x * info.colliderInfo.WorldScale.x),
		std::abs(info.Extent.y * info.colliderInfo.WorldScale.y),
		std::abs(info.Extent.z * info.colliderInfo.WorldScale.z)
	);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
void RigidBodyManager::CreateStaticBody(const SphereColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// ��ü �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ��ü �ݶ��̴��� ���� Shape ���� (�ִ� ���� ������ ���)
	float maxScale = std::abs(info.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = info.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
	{
		assert(false); // ���� �� ����� �ߴ�
	}
}
void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// ĸ�� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ĸ�� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	VPMath::Vector3 scale = info.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(info.HalfHeight * scale.y); // ���� ���� ����
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = info.Radius * maxScale; // ������ ����
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
	{
		assert(false); // ���� �� ����� �ߴ�
	}
}
void RigidBodyManager::CreateStaticBody(const ConvexColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// ������ �޽� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ������ �޽� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(info.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(info.colliderInfo.WorldScale.x),
		std::abs(info.colliderInfo.WorldScale.y),
		std::abs(info.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<StaticRigidBody> rigidBody = SettingStaticBody(shape, info, collidertype, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::BoxColliderInfo& info, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo)
{
	// �ڽ� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// �ڽ� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	physx::PxBoxGeometry mesh(
		std::abs(info.Extent.x * info.colliderInfo.WorldScale.x),
		std::abs(info.Extent.y * info.colliderInfo.WorldScale.y),
		std::abs(info.Extent.z * info.colliderInfo.WorldScale.z)
	);

	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, collidertype, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::SphereColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engininfo)
{
	// ��ü �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ��ü �ݶ��̴��� ���� Shape ���� (�ִ� ���� ������ ���)
	float maxScale = std::abs(info.colliderInfo.WorldScale.GetMaxComponent());
	float newRadius = info.Radius * maxScale;
	physx::PxSphereGeometry mesh(newRadius);
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engininfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// ĸ�� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ĸ�� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	VPMath::Vector3 scale = info.colliderInfo.WorldScale;
	VPMath::Vector2 size;
	size.y = std::abs(info.HalfHeight * scale.y); // ���� ���� ����
	scale.y = 0;
	float maxScale = std::abs(scale.GetMaxComponent());
	size.x = info.Radius * maxScale; // ������ ����
	physx::PxShape* shape = m_Physics->createShape(physx::PxCapsuleGeometry(size.x, size.y), *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engineinfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
void RigidBodyManager::CreateDynamicBody(const VPPhysics::ConvexColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// ������ �޽� �ݶ��̴��� ���� Material ����
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(info.colliderInfo.StaticFriction, info.colliderInfo.DynamicFriction, info.colliderInfo.Restitution);

	// ������ �޽� �ݶ��̴��� ���� Shape ���� (���� ������ �� ���)
	std::weak_ptr<ConvexMeshResource> convexMesh = m_ResourceManager.lock()->GetConvexMeshResource(info.FBXName);
	physx::PxConvexMesh* pxConvexMesh = convexMesh.lock()->GetConvexMesh();
	auto mesh = physx::PxConvexMeshGeometry(pxConvexMesh);
	mesh.scale.scale = {
		std::abs(info.colliderInfo.WorldScale.x),
		std::abs(info.colliderInfo.WorldScale.y),
		std::abs(info.colliderInfo.WorldScale.z)
	};
	physx::PxShape* shape = m_Physics->createShape(mesh, *pxMaterial);

	// ���� �ٵ� ����
	std::shared_ptr<DynamicRigidBody> rigidBody = SettingDynamicBody(shape, info, type, engineinfo);

	// ������ �ٵ� �ʱ�ȭ
	if (rigidBody->Initialize(shape, m_Physics))
	{
		m_RigidBodyMap[info.colliderInfo.EntityID] = rigidBody; // �ʿ� ������ �ٵ� ����
		AddBodyScene(rigidBody); // ���� ������ �ٵ� �߰�
		shape->release(); // Shape ����
	}
	else
		assert(false); // ���� �� ����� �ߴ�
}
#pragma endregion
#pragma region Setting Body

// ���� ������ �ٵ� �����ϴ� �Լ� (BoxColliderInfo ���)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// ���� ������ ���� (PhysicsLayer�� CollisionMatrix ���)
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer); // ���� ���� ���̾�
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)]; // �浹 ���
	shape->setSimulationFilterData(filterdata); // Shape�� ���� ������ ����

	// StaticRigidBody ���� �� ����
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody; // �ʿ� ���
	return staticBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (SphereColliderInfo ���)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody ���� �� ����
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (ConvexColliderInfo ���)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody ���� �� ����
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (CapsuleColliderInfo ���)
std::shared_ptr<StaticRigidBody> RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& type, const VPPhysics::PhysicsInfo& engininfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engininfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// StaticRigidBody ���� �� ����
	auto staticBody = std::make_shared<StaticRigidBody>(info, type, engininfo);
	m_RigidBodyMap[staticBody->GetID()] = staticBody;
	return staticBody;
}

// ���� ������ �ٵ� �����ϴ� �Լ� (BoxColliderInfo ���)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody ���� �� ����
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // �ʿ� ���
	return dynamicBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (SphereColliderInfo ���)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody ���� �� ����
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // �ʿ� ���
	return dynamicBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (ConvexColliderInfo ���)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody ���� �� ����
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // �ʿ� ���
	return dynamicBody;
}
// ���� ������ �ٵ� �����ϴ� �Լ� (CapsuleColliderInfo ���)
std::shared_ptr<DynamicRigidBody> RigidBodyManager::SettingDynamicBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& type, const PhysicsInfo& engineinfo)
{
	// ���� ������ ����
	physx::PxFilterData filterdata;
	filterdata.word0 = static_cast<int>(info.colliderInfo.PhysicsLayer);
	filterdata.word1 = engineinfo.CollisionMatrix[static_cast<int>(info.colliderInfo.PhysicsLayer)];
	shape->setSimulationFilterData(filterdata);

	// DynamicRigidBody ���� �� ����
	auto dynamicBody = std::make_shared<DynamicRigidBody>(info, type, engineinfo);
	m_RigidBodyMap[dynamicBody->GetID()] = dynamicBody; // �ʿ� ���
	return dynamicBody;
}

#pragma endregion

#pragma region Add_Release
// ������ �ٵ� ���� �߰��ϴ� �Լ�
void RigidBodyManager::AddBodyScene(std::shared_ptr<RigidBody> body)
{
	// ������ �ٵ� DynamicRigidBody���� Ȯ���ϰ� ���� �߰�
	if (IsDynamic(body->GetID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<DynamicRigidBody>(body)->GetPxDynamicRigid());
	// ������ �ٵ� StaticRigidBody���� Ȯ���ϰ� ���� �߰�
	else if (IsStatic(body->GetID()))
		m_Scene->addActor(*std::dynamic_pointer_cast<StaticRigidBody>(body)->GetPxStaticRigid());
}
// ������ �ٵ� ������ �����ϴ� �Լ�
void RigidBodyManager::ReleaseBodyScene(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID); // ��ƼƼ ID�� ������ �ٵ� �˻�
	if (it == m_RigidBodyMap.end())
		return; // ��ƼƼ�� ������ ��ȯ

	auto tempbody = it->second;

	// ������ �ٵ� Dynamic �Ǵ� Static���� Ȯ��
	if (!Reflection::IsSameType<DynamicRigidBody>(tempbody->GetTypeID()) &&
		!Reflection::IsSameType<StaticRigidBody>(tempbody->GetTypeID()))
		return;

	// ������ �ٵ� Ÿ�Կ� �°� ĳ����
	auto dynamicBody = std::dynamic_pointer_cast<DynamicRigidBody>(tempbody);
	auto staticBody = std::dynamic_pointer_cast<StaticRigidBody>(tempbody);

	// DynamicRigidBody�� ������ ����
	if (dynamicBody && dynamicBody->GetPxDynamicRigid()->getScene() == m_Scene)
	{
		m_Scene->removeActor(*dynamicBody->GetPxDynamicRigid());
		m_RigidBodyMap.erase(it); // �ʿ��� ����
	}
	// StaticRigidBody�� ������ ����
	else if (staticBody && staticBody->GetPxStaticRigid()->getScene() == m_Scene)
	{
		m_Scene->removeActor(*staticBody->GetPxStaticRigid());
		m_RigidBodyMap.erase(it); // �ʿ��� ����
	}
}
#pragma endregion

#pragma region Convert_Static_Dynamic
// Dynamic ������ �ٵ� Static���� ��ȯ�ϴ� �Լ�
void RigidBodyManager::ConvertToStatic(uint32_t EntityID)
{
	auto rigidbody = GetRigidBody(EntityID); // ������ �ٵ� �˻�
	if (!rigidbody)
		return; // ������ ��ȯ
	if (IsStatic(EntityID))
		return; // �̹� Static�̸� ��ȯ

	// Dynamic ������ �ٵ� ������ ����
	if (auto dynamicActor = std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody))
	{
		m_Scene->removeActor(*dynamicActor->GetPxDynamicRigid());
	}

	// Ÿ�Կ� ���� ������ Static ������ �ٵ� ����
	if (rigidbody->m_Boxinfo)
		CreateStaticBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Sphereinfo)
		CreateStaticBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Capulseinfo)
		CreateStaticBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Convexinfo)
		CreateStaticBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
}
// Static ������ �ٵ� Dynamic���� ��ȯ�ϴ� �Լ�
void RigidBodyManager::ConvertToDynamic(uint32_t EntityID)
{
	auto rigidbody = GetRigidBody(EntityID); // ������ �ٵ� �˻�
	if (!rigidbody)
		return; // ������ ��ȯ
	if (IsDynamic(EntityID))
		return; // �̹� Dynamic�̸� ��ȯ

	// Static ������ �ٵ� ������ ����
	if (auto staticActor = std::dynamic_pointer_cast<StaticRigidBody>(rigidbody))
		m_Scene->removeActor(*staticActor->GetPxStaticRigid());

	// Ÿ�Կ� ���� ������ Dynamic ������ �ٵ� ����
	if (rigidbody->m_Boxinfo)
		CreateDynamicBody(*rigidbody->m_Boxinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Sphereinfo)
		CreateDynamicBody(*rigidbody->m_Sphereinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Capulseinfo)
		CreateDynamicBody(*rigidbody->m_Capulseinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
	else if (rigidbody->m_Convexinfo)
		CreateDynamicBody(*rigidbody->m_Convexinfo, rigidbody->m_ColliderType, *rigidbody->m_PhysicInfo);
}
// Dynamic ������ �ٵ� Ư�� PhysicsLayer�� Static���� ��ȯ�ϴ� �Լ�
void RigidBodyManager::ConvertToStaticWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer)
{
	auto rigidbody = GetRigidBody(EntityID); // ������ �ٵ� �˻�
	if (!rigidbody)
		return; // ������ ��ȯ
	if (IsStatic(EntityID))
		return; // �̹� Static�̸� ��ȯ

	// Dynamic ������ �ٵ� ������ ����
	if (auto dynamicActor = std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody))
		m_Scene->removeActor(*dynamicActor->GetPxDynamicRigid());

	// Ÿ�Կ� ���� ������ PhysicsLayer�� �����ϰ� Static ������ �ٵ� ����
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
// Static ������ �ٵ� Ư�� PhysicsLayer�� Dynamic���� ��ȯ�ϴ� �Լ�
void RigidBodyManager::ConvertToDynamicWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer)
{
	auto rigidbody = GetRigidBody(EntityID); // ������ �ٵ� �˻�
	if (!rigidbody)
		return; // ������ ��ȯ
	if (IsDynamic(EntityID))
		return; // �̹� Dynamic�̸� ��ȯ


	// Static ������ �ٵ� ������ ����
	if (auto staticActor = std::dynamic_pointer_cast<StaticRigidBody>(rigidbody))
		m_Scene->removeActor(*staticActor->GetPxStaticRigid());

	// Ÿ�Կ� ���� ������ PhysicsLayer�� �����ϰ� Dynamic ������ �ٵ� ����
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


// ������ �ٵ� �������� �Լ�
std::shared_ptr<RigidBody> RigidBodyManager::GetRigidBody(uint32_t entityID)
{
	auto it = m_RigidBodyMap.find(entityID); // ��ƼƼ ID�� �˻�
	if (it == m_RigidBodyMap.end())
		return nullptr; // ������ nullptr ��ȯ
	return it->second; // ������ �ٵ� ��ȯ
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

// Ư�� ���� ���̾�� ���� ������ ���� �����ϴ� �Լ�
void RigidBodyManager::ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!scene)
		return; // ���� ��ȿ���� ������ ��ȯ

	// ������ Static Rigid Actor�� ������
	PxU32 actorCount = scene->getNbActors(physx::PxActorTypeFlag::eRIGID_STATIC);
	std::vector<PxRigidStatic*> actors(actorCount);
	scene->getActors(physx::PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(actors.data()), actorCount);

	// �� Static Actor�� ��ȸ
	for (PxRigidStatic* actor : actors)
	{
		if (!actor)
			continue;

		// Actor���� Shapes�� ������
		PxU32 shapeCount = actor->getNbShapes();
		std::vector<PxShape*> shapes(shapeCount);
		actor->getShapes(shapes.data(), shapeCount);

		for (PxShape* shape : shapes)
		{
			// Simulation Shape�̸鼭 Trigger Shape�� �ƴ� ��� ó��
			if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
			{
				// ���� �����͸� Ȯ���Ͽ� ���̾�� ��
				PxFilterData filterData = shape->getSimulationFilterData();
				if (filterData.word0 == static_cast<PxU32>(layer))
				{
					// �ش� Actor�� ���� �� �� �����͸� ����
					RigidBodyManager::ExtractVerticesAndFaces(actor, outVertices, outIndices);
					break; // �ϳ��� Shape�� ó���ϸ� Actor ó���� �ߴ�
				}
			}
		}
	}
}

// Ư�� ��ƼƼ�� ������ ���� �����ϴ� �Լ�
void RigidBodyManager::ExtractEntityVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!HasRigidBody(entityID))
		return; // ��ƼƼ�� ������ �ٵ� ������ ��ȯ

	if (IsStatic(entityID)) // Static Rigid Body���� Ȯ��
	{
		auto staticBody = dynamic_cast<StaticRigidBody*>(GetRigidBody(entityID).get());
		physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
		if (!pxRigidStatic)
			return;
		ExtractVerticesAndFaces(pxRigidStatic, outVertices, outIndices);
	}
	else if (IsDynamic(entityID)) // Dynamic Rigid Body���� Ȯ��
	{
		auto dynamicrigid = dynamic_cast<DynamicRigidBody*>(GetRigidBody(entityID).get());
		physx::PxRigidDynamic* pxRigidDynamic = dynamicrigid->GetPxDynamicRigid();
		if (!pxRigidDynamic)
			return;
		ExtractVerticesAndFaces(pxRigidDynamic, outVertices, outIndices);
	}
}

// Rigid Actor���� ������ ���� �����ϴ� �Լ�
void RigidBodyManager::ExtractVerticesAndFaces(PxRigidActor* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices)
{
	if (!actor)
		return; // Actor�� ��ȿ���� ������ ��ȯ

	// Actor�� ���� ��ȯ(���� ��ǥ��)�� ������
	PxTransform globalPose = actor->getGlobalPose();

	// Actor���� Shapes�� ������
	PxU32 shapeCount = actor->getNbShapes();
	std::vector<PxShape*> shapes(shapeCount);
	actor->getShapes(shapes.data(), shapeCount);

	for (PxShape* shape : shapes)
	{
		// Simulation Shape���� ǥ�õ� �͸� ó��
		if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			// Shape�� ������Ʈ���� ������
			PxGeometryHolder geometryHolder = shape->getGeometry();

			if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH) // ������ �޽� ó��
			{
				// Convex Mesh Geometry ��������
				PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
				PxConvexMesh* convexMesh = convexGeometry.convexMesh;

				// Convex Mesh ������ ��������
				PxMeshScale scale = convexGeometry.scale;

				// ���� ������ ����
				const PxVec3* meshVertices = convexMesh->getVertices();
				PxU32 vertexCount = convexMesh->getNbVertices();

				// ���� ���� �������� ���� �ε��� ����
				int baseIndex = static_cast<int>(outVertices.size());

				// ������ ��ȯ�ϰ� ���� ��ǥ��� ��ȯ �� �߰�
				for (PxU32 i = 0; i < vertexCount; ++i)
				{
					PxVec3 scaledVertex = scale.transform(meshVertices[i]); // ������ ����
					PxVec3 worldVertex = globalPose.transform(scaledVertex); // ���� ��ȯ
					outVertices.emplace_back(worldVertex.x, worldVertex.y, worldVertex.z); // ��� ���ۿ� �߰�
				}

				// �� ������ ���� (�ε��� ���� ���)
				const PxU8* indexBuffer = convexMesh->getIndexBuffer();
				PxU32 polygonCount = convexMesh->getNbPolygons();

				for (PxU32 i = 0; i < polygonCount; ++i)
				{
					PxHullPolygon polygon;
					convexMesh->getPolygonData(i, polygon);

					// �������� ���� �ε��� ó��
					if (polygon.mNbVerts == 3) // �ﰢ���� ��� �ٷ� ó��
						for (PxU32 j = 0; j < 3; ++j)
						{
							PxU32 vertexIndex = indexBuffer[polygon.mIndexBase + j];
							outIndices.push_back(baseIndex + static_cast<int>(vertexIndex));
						}
					else if (polygon.mNbVerts > 3) // �ٰ����� ��� �ﰢ��ȭ ó��
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

// Rigid Actor���� ���� ������ �����ϴ� �Լ�
void RigidBodyManager::ExtractLocalVerticesFromActor(physx::PxRigidActor* actor, std::vector<VPMath::Vector3>& localVertices)
{
	if (!actor)
		return; // Actor�� ��ȿ���� ������ ��ȯ
	// Actor���� Shapes�� ������
	PxU32 shapeCount = actor->getNbShapes();
	std::vector<PxShape*> shapes(shapeCount);
	actor->getShapes(shapes.data(), shapeCount);
	for (PxShape* shape : shapes)
	{
		// Simulation Shape���� ǥ�õ� �͸� ó��
		if (shape->getFlags().isSet(physx::PxShapeFlag::eSIMULATION_SHAPE))
		{
			PxGeometryHolder geometryHolder = shape->getGeometry();

			// Convex Mesh���� Ȯ��
			if (geometryHolder.getType() == PxGeometryType::eCONVEXMESH)
			{
				PxConvexMeshGeometry convexGeometry = geometryHolder.convexMesh();
				PxConvexMesh* convexMesh = convexGeometry.convexMesh;

				if (!convexMesh)
					continue;

				// ���� ���� ������ ����
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
		// StaticRigidBody ó��
		auto staticBody = dynamic_cast<StaticRigidBody*>(rigidBody.get());
		if (!staticBody)
			return localVertices;

		physx::PxRigidStatic* pxRigidStatic = staticBody->GetPxStaticRigid();
		if (!pxRigidStatic)
			return localVertices;

		// PxRigidStatic���� ���� ��������
		ExtractLocalVerticesFromActor(pxRigidStatic, localVertices);
	}
	else if (IsDynamic(entityID))
	{
		// DynamicRigidBody ó��
		auto dynamicBody = dynamic_cast<DynamicRigidBody*>(rigidBody.get());
		if (!dynamicBody)
			return localVertices;

		physx::PxRigidDynamic* pxRigidDynamic = dynamicBody->GetPxDynamicRigid();
		if (!pxRigidDynamic)
			return localVertices;

		// PxRigidDynamic���� ���� ��������
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
		float length = Dir.Length(); // ������ ũ�⸦ Ȯ��
		// ������ ũ�Ⱑ 0�̸� ���� ó��
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
		float length = Dir.Length(); // ������ ũ�⸦ Ȯ��

		// ������ ũ�Ⱑ 0�̰ų� ���� ���� ���
		if (length < 1e-6f || V == 0.0f) {
			dynamicBody->GetPxDynamicRigid()->setLinearVelocity({0,0,0}); // �ӵ� 0���� ����
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
		tempActor->getGlobalPose().p,				// ������
		tempDir,									// ��������
		max,										// �Ÿ�
		buf);										// PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) { return a.distance < b.distance; });
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
		PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
		PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������

		auto tempID = FindIDByActor(buf.touches[i].actor);	// i��° ���˵� ����(���� ��ü)�� ���õ� ��ƼƼ�� ID�� ã��
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
	auto tempActor = FindActorByID(entityID);			// entityID�� ���� ���� ���͸� ã�ƿ�
	if (!tempActor)
		return {};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };	// �Էµ� ���� ���͸� PhysX�� PxVec3�� ��ȯ
	tempDir.normalize();								// ���� ���͸� ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;						// ����ĳ��Ʈ�� �ִ� �Ÿ��� ����
	const PxU32 bufferSize = 32;						// ����ĳ��Ʈ ��Ʈ ����� ������ ������ ũ�� ����
	PxRaycastHit hitBuffer[bufferSize];					// ����ĳ��Ʈ�� ���� ���˵� ��ü���� ������ ������ ����
	PxRaycastBuffer buf(hitBuffer, bufferSize);			// ����ĳ��Ʈ ����� ������ ���� ��ü ����

	if (tempDir.isZero())	// ���� ���Ͱ� 0�� ���(����ȭ ����), 0�� ��ȯ
		return raycastresult;

	// ����ĳ��Ʈ ������ ����: ������ ���� ��ġ�� �������� ���� ��
	PxVec3 raypos = PxVec3{ offset.x, offset.y, offset.z } + tempActor->getGlobalPose().p;

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,               // ����ĳ��Ʈ ������
		tempDir,              // ����ĳ��Ʈ ���� (����ȭ�� ���� ����)
		max,                  // ����ĳ��Ʈ �ִ� �Ÿ�
		buf);                 // ����ĳ��Ʈ ����� ������ ����

	if (!find)  // ����ĳ��Ʈ�� �ƹ��͵� ã�� ���� ��� 0�� ��ȯ
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ����ĳ��Ʈ�� ���˵� ��� ��ü�鿡 ���� �ݺ�
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;   // i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
		PxVec3 n = buf.getTouch(i).normal;     // i��° ���˵� ������ ǥ�� ���� ���͸� ������
		PxF32 d = buf.getTouch(i).distance;    // i��° ���˵� ���������� �Ÿ��� ������
		auto tempID = FindIDByActor(buf.touches[i].actor);  // i��° ���˵� ����(���� ��ü)�� �ش��ϴ� ��ƼƼ ID�� ã��

		if (tempID == entityID)  // �ڽŰ� ������ ��ƼƼ ID�� �����ϰ� ���� �ݺ����� �Ѿ
			continue;

		// Ʈ���Ű� �ƴ� �浹ü�� ���ؼ��� �ش� ��ƼƼ ID�� ��ȯ
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
		{

			 raycastresult={ tempID ,{p.x,p.y,p.z},{n.x,n.y,n.z} ,d };
			return raycastresult;
		}
	}

	return 	 raycastresult;  // Ʈ���Ű� �ƴ� �浹ü�� ã�� ���� ��� 0�� ��ȯ
}
RaycastData RigidBodyManager::RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance)
{
	RaycastData raycastresult{};
	physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
	tempDir.normalize();  // ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// �־��� ��ġ�� PhysX�� PxVec3�� ��ȯ
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,                                   // ������
		tempDir,                                  // ��������
		max,                                      // �Ÿ�
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ��Ʈ�� ��ƼƼ�� �߿��� Ʈ���Ű� �ƴ� �浹ü�� ID�� ��ȯ
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
		PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
		PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������
		auto tempID = FindIDByActor(buf.touches[i].actor);	// i��° ���˵� ����(���� ��ü)�� ���õ� ��ƼƼ�� ID�� ã��

		// Ʈ���Ű� �ƴ� �浹ü�� ��쿡�� ��ƼƼ ID ��ȯ
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
	tempDir.normalize();  // ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// �־��� ��ġ�� PhysX�� PxVec3�� ��ȯ
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,                                   // ������
		tempDir,                                  // ��������
		max,                                      // �Ÿ�
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ��Ʈ�� ��ƼƼ�� �߿��� Ʈ���Ű� �ƴ� �浹ü�� ID�� ��ȯ
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{

		auto tempID = FindIDByActor(buf.touches[i].actor);	// i��° ���˵� ����(���� ��ü)�� ���õ� ��ƼƼ�� ID�� ã��

		PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
		PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
		PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������
		if (tempID == entityID)  // �ڽŰ� ������ ��ƼƼ ID�� �����ϰ� ���� �ݺ����� �Ѿ
			continue;
		// Ʈ���Ű� �ƴ� �浹ü�� ��쿡�� ��ƼƼ ID ��ȯ
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
	tempDir.normalize();  // ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return temp;

	// �־��� ��ġ�� PhysX�� PxVec3�� ��ȯ
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,                                   // ������
		tempDir,                                  // ��������
		max,                                      // �Ÿ�
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return temp;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ��Ʈ�� ��ƼƼ�� �߿��� Ʈ���Ű� �ƴ� �浹ü�� ID�� ��ȯ
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		auto tempID = FindIDByActor(buf.touches[i].actor); // i��° ���˵� ���Ϳ� ���õ� ��ƼƼ ID

		// entityIDs�� ���Ե� ID�� ����
		if (std::find(entityIDs.begin(), entityIDs.end(), tempID) != entityIDs.end())
			continue;

		// Ʈ���Ű� �ƴ� �浹ü�� ��쿡�� ó��
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)) {
			PxVec3 p = buf.getTouch(i).position;  // ���˵� ������ ��ġ
			PxVec3 n = buf.getTouch(i).normal;    // ���˵� ������ ���� ����
			PxF32 d = buf.getTouch(i).distance;   // ���˵� ���������� �Ÿ�
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
	tempDir.normalize();  // ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// �־��� ��ġ�� PhysX�� PxVec3�� ��ȯ
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,                                   // ������
		tempDir,                                  // ��������
		max,                                      // �Ÿ�
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ��Ʈ�� ��ƼƼ�� �߿��� Ʈ���Ű� �ƴ� �浹ü�� ID�� ��ȯ
	for (PxU32 i = 0; i < buf.nbTouches; i++)
	{
		PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
		PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
		PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������
		auto tempID = FindIDByActor(buf.touches[i].actor);	// i��° ���˵� ����(���� ��ü)�� ���õ� ��ƼƼ�� ID�� ã��

		// Ʈ���Ű� �ƴ� �浹ü�� ��쿡�� ��ƼƼ ID ��ȯ
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
	tempDir.normalize();  // ���� ���ͷ� ����ȭ
	PxF32 max = (PxF32)distance;
	const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
	PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
	PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here

	if (tempDir.isZero())
		return raycastresult;

	// �־��� ��ġ�� PhysX�� PxVec3�� ��ȯ
	PxVec3 raypos = PxVec3{ location.x, location.y, location.z };

	// ����ĳ��Ʈ ����
	bool find = m_Scene->raycast(
		raypos,                                   // ������
		tempDir,                                  // ��������
		max,                                      // �Ÿ�
		buf);                                     // PxRaycastCallback& hitCall

	if (!find)
		return raycastresult;
	std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
		return a.distance < b.distance;
		});
	// ��Ʈ�� ��ƼƼ�� �߿��� Ʈ���Ű� �ƴ� �浹ü�� ID�� ��ȯ
	for (PxU32 i = 0; i < buf.nbTouches; i++) {
		auto tempID = FindIDByActor(buf.touches[i].actor); // i��° ���˵� ���Ϳ� ���õ� ��ƼƼ ID

		// entityIDs�� ���Ե� ID�� ����
		if (std::find(entityIDs.begin(), entityIDs.end(), tempID) != entityIDs.end())
			continue;

		// Ʈ���Ű� �ƴ� �浹ü�� ��쿡�� ó��
		if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)) {
			PxVec3 p = buf.getTouch(i).position;  // ���˵� ������ ��ġ
			PxVec3 n = buf.getTouch(i).normal;    // ���˵� ������ ���� ����
			PxF32 d = buf.getTouch(i).distance;   // ���˵� ���������� �Ÿ�
			raycastresult.push_back({ tempID, {p.x, p.y, p.z}, {n.x, n.y, n.z}, d }) ;
		}
	}
	return raycastresult;
}



physx::PxRigidActor* RigidBodyManager::FindActorByID(uint32_t entityID)
{
	// ���� m_RigidBodyMap���� ��ƼƼ�� ã��
	auto it = m_RigidBodyMap.find(entityID);
	if (it == m_RigidBodyMap.end())
		return nullptr;

	auto rigidbody = it->second;  // �˻��� ����� ���� ������
	// DynamicRigidBody Ÿ������ ���� üũ �� ��ȯ
	if (IsDynamic(entityID))
		return std::dynamic_pointer_cast<DynamicRigidBody>(rigidbody)->GetPxDynamicRigid();

	// StaticRigidBody Ÿ������ üũ �� ��ȯ
	if (IsStatic(entityID))
		return std::dynamic_pointer_cast<StaticRigidBody>(rigidbody)->GetPxStaticRigid();
	return nullptr;  // �ش��ϴ� Ÿ���� ������ nullptr ��ȯ
}

