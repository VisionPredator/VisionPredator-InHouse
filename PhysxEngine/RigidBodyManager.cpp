#include "pch.h"
#include "RigidBodyManager.h"
#include "StaticRigidBody.h"

RigidBodyManager::RigidBodyManager()
{

}
RigidBodyManager::~RigidBodyManager()
{
	m_RigidBodies.clear();
}


bool RigidBodyManager::Initialize(physx::PxPhysics* physics, PhysxColliManager* CollManager)
{
	m_Physics = physics;
	m_PhysxCollisionManager = CollManager;
	return true;
}

void RigidBodyManager::Update()
{

}

void RigidBodyManager::CreateStaticBody(const BoxColliderInfo boxinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
	physx::PxMaterial* pxMaterial = m_Physics->createMaterial(boxinfo.colliderInfo.StaticFriction, boxinfo.colliderInfo.DynamicFriction, boxinfo.colliderInfo.Restitution);
	physx::PxShape* shape = m_Physics->createShape(physx::PxBoxGeometry(boxinfo.Extent.x, boxinfo.Extent.y, boxinfo.Extent.z), *pxMaterial);

	StaticRigidBody* rigidBody = SettingStaticBody(shape, boxinfo.colliderInfo, collidertype, engininfo);
}

void RigidBodyManager::CreateStaticBody(const SphereColliderInfo sphereinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
}

void RigidBodyManager::CreateStaticBody(const CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const PhysicsInfo engininfo)
{
}

void RigidBodyManager::CreateDynamicBody()
{
}

StaticRigidBody* RigidBodyManager::SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo engininfo)
{
	physx::PxFilterData filterdata;
	filterdata.word0 = info.LayerNumber;
	filterdata.word1 = engininfo.CollisionMatrix[info.LayerNumber];
	shape->setSimulationFilterData(filterdata);

	StaticRigidBody* staticBody = new StaticRigidBody(colliderType, info.EntityID, info.LayerNumber);

}

