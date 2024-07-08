#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class StaticRigidBody :
	public RigidBody
{
public:
	StaticRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber);
	void Initialize(ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics, CollisionData* data);
	~StaticRigidBody()=default;
	physx::PxRigidStatic* GetPxStaticRigid();
private:
	physx::PxRigidStatic* m_StaticRigid{};

};

physx::PxRigidStatic* StaticRigidBody::GetPxStaticRigid()
{
	return m_StaticRigid;
}