#pragma once
#include "RigidBody.h"
class DynamicRigidBody :
	public RigidBody
{
public:
	DynamicRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber);
	~DynamicRigidBody();
	bool Initialize(VPPhysics::ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics);
	inline physx::PxRigidDynamic* GetPxDynamicRigid();
	entt::id_type GetTypeID() const override {
		return Reflection::GetTypeID<DynamicRigidBody>();
	}
private:
	physx::PxRigidDynamic* m_DynamicRigid{};

};



physx::PxRigidDynamic* DynamicRigidBody::GetPxDynamicRigid()
{
	return m_DynamicRigid;
}
