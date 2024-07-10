#include "pch.h"
#include "DynamicRigidBody.h"

DynamicRigidBody::DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber)
	:RigidBody(colltype, entityId, layerNumber)
{
}

