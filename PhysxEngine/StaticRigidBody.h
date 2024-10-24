#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class StaticRigidBody :
	public RigidBody
{
public:
	StaticRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber);
	bool Initialize(ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics);
	~StaticRigidBody();
	inline physx::PxRigidStatic* GetPxStaticRigid();
	entt::id_type GetTypeID() const override {
		return Reflection::GetTypeID<StaticRigidBody>();
	}
private:
	physx::PxRigidStatic* m_StaticRigid{};
	

};

physx::PxRigidStatic* StaticRigidBody::GetPxStaticRigid()
{
	return m_StaticRigid;
}