#pragma once
#include "VPPhysicsStructs.h"
#include "EventSubscriber.h"
class CollisionManager;
class RigidBody;
using namespace VPPhysics;
class StaticRigidBody;
class DynamicRigidBody;
class PhysichResourceManager;
class RigidBodyManager:public EventSubscriber
{
public: 
	RigidBodyManager();
	~RigidBodyManager();
	bool Initialize(physx::PxPhysics* physics, physx::PxScene* Scene, std::shared_ptr<PhysichResourceManager> resourceManager);
	void Update();
	void EndScene();
	void CreateStaticBody(const VPPhysics::BoxColliderInfo& boxinfo, const  EColliderType& collidertype,const VPPhysics::PhysicsInfo& engininfo );
	void CreateStaticBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	void CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const VPPhysics::PhysicsInfo& engininfo);
	std::shared_ptr<StaticRigidBody> SettingStaticBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);
	std::shared_ptr<DynamicRigidBody> SettingDynamicBody(physx::PxShape* shape, const ColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);
	void ReleaseBodyScene(uint32_t EntityID);
	std::shared_ptr<RigidBody> GetRigidBody(uint32_t EntityID);
	
	bool HasRigidBody(uint32_t EntityID);
	void ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractVerticesAndFaces(PxRigidStatic* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);

	void SetGobalPose(uint32_t entityID, const VPMath::Vector3& P, const VPMath::Quaternion& Q);
	VPMath::Vector3 GetVelocity(uint32_t entityID);
	void AddVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V);
	VPMath::Vector3 GetGobalLocation(uint32_t entityID);
	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID);



private:
	void OnAddBodyScene(std::shared_ptr<RigidBody> rigidbody);
	void OnReleaseBodyScene(std::any data);
	void AddBodyScene(std::shared_ptr<RigidBody> body);

	std::weak_ptr<CollisionManager> m_Collsion;

	physx::PxPhysics* m_Physics = nullptr;
	physx::PxScene* m_Scene=nullptr;
	std::unordered_map<uint32_t, std::shared_ptr<RigidBody>> m_RigidBodyMap{};
	std::weak_ptr<PhysichResourceManager> m_ResourceManager;

};

