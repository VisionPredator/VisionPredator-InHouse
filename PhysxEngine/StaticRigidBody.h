#pragma once
#include "RigidBody.h"
class StaticRigidBody :
	public RigidBody
{
public:
	StaticRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, uint32_t layerNumber);
	~StaticRigidBody()=default;
	physx::PxRigidStatic* GetPxStaticRigid();
private:
	physx::PxRigidStatic* m_StaticRigid{};

};

physx::PxRigidStatic* StaticRigidBody::GetPxStaticRigid()
{
	return m_StaticRigid;
}