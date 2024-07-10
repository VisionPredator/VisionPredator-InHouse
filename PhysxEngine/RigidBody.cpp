#include "pch.h"
#include "RigidBody.h"


RigidBody::RigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber)
	: m_ColliderType{ colltype }
	, m_EntityID{ entityId }
	, m_LayerNum{ layerNumber }
{
}

RigidBody::~RigidBody()
{
}
