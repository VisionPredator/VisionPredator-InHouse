#include "pch.h"
#include "StaticRigidBody.h"


StaticRigidBody::StaticRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber)
	:RigidBody(colltype, entityId, layerNumber)
{
}

void StaticRigidBody::Initialize(ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics, CollisionData* data)
{
}

