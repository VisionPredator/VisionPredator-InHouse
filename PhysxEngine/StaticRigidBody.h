#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class StaticRigidBody :
	public RigidBody
{
public:
	StaticRigidBody(BoxColliderInfo info, EColliderType, const PhysicsInfo& engininfo);
	StaticRigidBody(CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	StaticRigidBody(SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	StaticRigidBody(ConvexColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	bool Initialize(physx::PxShape* shape, physx::PxPhysics* physics);
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