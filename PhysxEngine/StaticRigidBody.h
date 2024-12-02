#pragma once
#include "RigidBody.h"
using namespace VPPhysics;
class StaticRigidBody :
	public RigidBody
{
public:
	// Collider의 자료형에 따라 Rigid 형태를 생성하도록 설정
	StaticRigidBody(BoxColliderInfo info, EColliderType, const PhysicsInfo& engininfo);
	// Collider의 자료형에 따라 Rigid 형태를 생성하도록 설정
	StaticRigidBody(CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider의 자료형에 따라 Rigid 형태를 생성하도록 설정
	StaticRigidBody(SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo);
	// Collider의 자료형에 따라 Rigid 형태를 생성하도록 설정
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