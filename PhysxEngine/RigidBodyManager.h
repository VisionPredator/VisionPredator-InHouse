#pragma once
#include "VPPhysicsStructs.h"
class PhysxColliManager;
class RigidBody;
using namespace VPPhysics;
class StaticRigidBody;
class RigidBodyManager
{
public: 
	RigidBodyManager();
	~RigidBodyManager();
	bool Initialize(physx::PxPhysics* physics,	PhysxColliManager* CollManager);
	void Update();
	void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype,const VPPhysics::PhysicsInfo engininfo );
	void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateDynamicBody();
	StaticRigidBody* SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo engininfo);


private:
	physx::PxPhysics* m_Physics{};
	PhysxColliManager* m_PhysxCollisionManager{};
	std::unordered_map<uint32_t, RigidBody*> m_RigidBodies{};

};

