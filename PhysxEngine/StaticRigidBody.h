#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class StaticRigidBody :
	public RigidBody
{
public:
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	StaticRigidBody(BoxColliderInfo info, EColliderType, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	StaticRigidBody(CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	StaticRigidBody(SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider�� �ڷ����� ���� Rigid ���¸� �����ϵ��� ����
	StaticRigidBody(ConvexColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	bool Initialize(physx::PxShape* shape, physx::PxPhysics* physics);
	~StaticRigidBody();
	inline physx::PxRigidStatic* GetPxStaticRigid();
	entt::id_type GetTypeID() const override { return Reflection::GetTypeID<StaticRigidBody>(); }
private:
	physx::PxRigidStatic* m_StaticRigid{};
};

physx::PxRigidStatic* StaticRigidBody::GetPxStaticRigid()
{
	return m_StaticRigid;
}