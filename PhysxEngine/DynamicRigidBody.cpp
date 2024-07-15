#include "pch.h"
#include "DynamicRigidBody.h"
using namespace VPPhysics;
DynamicRigidBody::DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber)
	:RigidBody(colltype, entityId, layerNumber)
{
}

bool DynamicRigidBody::Initialize(VPPhysics::ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics, VPPhysics::CollisionData* data)
{
	if (m_ColliderType == EColliderType::COLLISION)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	data->myId = m_EntityID;
	data->myLayerNumber = m_LayerNum;
	shape->userData = data;
	shape->setContactOffset(0.02f);
	shape->setRestOffset(0.01f);
	physx::PxTransform transform;
	transform.p = {
		colliderInfo.WorldLocation.x,
		colliderInfo.WorldLocation.y,
		colliderInfo.WorldLocation.z
	};
	transform.q.x = colliderInfo.WorldQuaternion.x;
	transform.q.y = colliderInfo.WorldQuaternion.y;
	transform.q.z = colliderInfo.WorldQuaternion.z;
	transform.q.w = colliderInfo.WorldQuaternion.w;



	m_DynamicRigid = physics->createRigidDynamic(transform);

	// LOCK ¼³Á¤	
	m_DynamicRigid->setRigidDynamicLockFlags(
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[0] << 0) |
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[1] << 1) |
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.LinearLock[2] << 2) |
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[0] << 3) |
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[1] << 4) |
		(PxRigidDynamicLockFlag::Enum)(colliderInfo.AngleLock[2] << 5));

	m_DynamicRigid->userData = data;
	if (m_DynamicRigid == nullptr)
		return false;

	if (!m_DynamicRigid->attachShape(*shape))
	{
		assert(shape->getReferenceCount() == 1);
		return false;

	}
	physx::PxRigidBodyExt::updateMassAndInertia(*m_DynamicRigid, colliderInfo.Density);
	return true;
}
