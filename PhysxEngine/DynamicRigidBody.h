#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class DynamicRigidBody :
	public RigidBody
{
public:
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	DynamicRigidBody(BoxColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	DynamicRigidBody(CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	DynamicRigidBody(SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	DynamicRigidBody(ConvexColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	~DynamicRigidBody();
	bool Initialize(physx::PxShape* shape, physx::PxPhysics* physics);
	inline physx::PxRigidDynamic* GetPxDynamicRigid();
	entt::id_type GetTypeID() const override { return Reflection::GetTypeID<DynamicRigidBody>(); }
private:
	physx::PxRigidDynamic* m_DynamicRigid{};
};

physx::PxRigidDynamic* DynamicRigidBody::GetPxDynamicRigid()
{
	return m_DynamicRigid;
}
