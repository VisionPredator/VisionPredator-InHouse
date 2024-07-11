#pragma once
#include "RigidBody.h"
class DynamicRigidBody :
	public RigidBody
{
public:
	DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber);
	~DynamicRigidBody() = default;
	bool Initialize(VPPhysics::ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics, VPPhysics::CollisionData* data);
	inline physx::PxRigidDynamic* GetPxDynamicRigid();
private:
	physx::PxRigidDynamic* m_DynamicRigid{};
};



physx::PxRigidDynamic* DynamicRigidBody::GetPxDynamicRigid()
{
	return m_DynamicRigid;
}
