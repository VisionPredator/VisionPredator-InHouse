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
	bool Initialize(physx::PxPhysics* physics, physx::PxScene* Scene);
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
	std::shared_ptr<RigidBody> GetRigidBody(uint32_t EntityID);
	bool HasRigidBody(uint32_t EntityID);

	void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q);
	VPMath::Vector3 GetVelocity(uint32_t entityID);
	void AddVelocity(uint32_t entityID, VPMath::Vector3 dir, float V);
	VPMath::Vector3 GetGobalLocation(uint32_t entityID);
	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID);



private:
	void OnAddBodyScene(std::any data);
	void OnReleaseBodyScene(std::any data);
	void AddBodyScene(RigidBody* body);

	std::weak_ptr<CollisionManager> m_Collsion;

	physx::PxPhysics* m_Physics = nullptr;
	physx::PxScene* m_Scene=nullptr;
	std::unordered_map<uint32_t, std::shared_ptr<RigidBody>> m_RigidBodyMap{};

};

