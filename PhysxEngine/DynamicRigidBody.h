#pragma once
#include "RigidBody.h"
class DynamicRigidBody :
	public RigidBody
{
public:
	DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber);
	~DynamicRigidBody() = default;
	physx::PxRigidDynamic* GetPxDynamicRigid();
private:
	physx::PxRigidDynamic* m_DynamicRigid{};
};


physx::PxRigidDynamic* DynamicRigidBody::GetPxDynamicRigid()
{
	return m_DynamicRigid;
}
