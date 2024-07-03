#include "pch.h"
#include "DynamicRigidBody.h"

DynamicRigidBody::DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber)
	:RigidBody(colltype, entityId, layerNumber)
{
}

