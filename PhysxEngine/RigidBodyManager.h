#pragma once
#include "VPPhysicsStructs.h"
#include "../VPEngine/EventSubscriber.h"
class CollisionManager;
class RigidBody;
using namespace VPPhysics;
class StaticRigidBody;
class DynamicRigidBody;
class RigidBodyManager:public EventSubscriber
{
public: 
	RigidBodyManager();
	~RigidBodyManager();
	bool Initialize(physx::PxPhysics* physics, physx::PxScene* Scene,	CollisionManager* CollManager);
	void Update();
	void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype,const VPPhysics::PhysicsInfo engininfo );
	void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype, const VPPhysics::PhysicsInfo engininfo);
	StaticRigidBody* SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo engininfo);
	DynamicRigidBody* SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo engininfo);
	void ReleaseBodyScene(uint32_t EntityID);


private:
	void OnAddBodyScene(std::any data);
	void OnReleaseBodyScene(std::any data);
	void AddBodyScene(RigidBody* body);


	physx::PxPhysics* m_Physics = nullptr;
	physx::PxScene* m_Scene=nullptr;
	CollisionManager* m_PhysxCollisionManager = nullptr;
	std::unordered_map<uint32_t, RigidBody*> m_RigidBodyMap{};

};

