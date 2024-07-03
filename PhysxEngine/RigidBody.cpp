#include "pch.h"
#include "RigidBody.h"


RigidBody::RigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber)
	: m_ColliderType{ colltype }
	, m_EntityID{ entityId }
	, m_LayerNum{ layerNumber }
{
}

RigidBody::~RigidBody()
{
}
