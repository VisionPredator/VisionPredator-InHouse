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
	void CreateStaticBody(const BoxColliderInfo& boxinfo, const  EColliderType& collidertype,const PhysicsInfo& engininfo );
	void CreateStaticBody(const SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateStaticBody(const CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateStaticBody(const ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateDynamicBody(const BoxColliderInfo& boxinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateDynamicBody(const SphereColliderInfo& sphereinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateDynamicBody(const CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);
	void CreateDynamicBody(const ConvexColliderInfo& convexMeshinfo, const EColliderType& collidertype, const PhysicsInfo& engininfo);

	std::shared_ptr<StaticRigidBody> SettingStaticBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);
	std::shared_ptr<StaticRigidBody> SettingStaticBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);
	std::shared_ptr<StaticRigidBody> SettingStaticBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);
	std::shared_ptr<StaticRigidBody> SettingStaticBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& colliderType, const VPPhysics::PhysicsInfo& engininfo);

	std::shared_ptr<DynamicRigidBody> SettingDynamicBody(physx::PxShape* shape, const BoxColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo& engininfo);
	std::shared_ptr<DynamicRigidBody> SettingDynamicBody(physx::PxShape* shape, const SphereColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo& engininfo);
	std::shared_ptr<DynamicRigidBody> SettingDynamicBody(physx::PxShape* shape, const ConvexColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo& engininfo);
	std::shared_ptr<DynamicRigidBody> SettingDynamicBody(physx::PxShape* shape, const CapsuleColliderInfo& info, const EColliderType& colliderType, const PhysicsInfo& engininfo);
	void ReleaseBodyScene(uint32_t EntityID);
	std::shared_ptr<RigidBody> GetRigidBody(uint32_t EntityID);
	void ConvertToStatic(uint32_t EntityID);
	void ConvertToDynamic(uint32_t EntityID);
	void ConvertToStaticWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer);
	void ConvertToDynamicWithLayer(uint32_t EntityID, VPPhysics::EPhysicsLayer layer);
	bool HasRigidBody(uint32_t EntityID);
	bool IsDynamic(uint32_t EntityID);
	bool IsStatic(uint32_t EntityID);
	void ExtractSceneVerticesAndFacesByLayer(PxScene* scene, EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractVerticesAndFaces(PxRigidActor* actor, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractLocalVerticesFromActor(physx::PxRigidActor* actor, std::vector<VPMath::Vector3>& localVertices);
	std::vector<VPMath::Vector3> GetConVexMeshVertex(uint32_t entityid);

	void SetGobalPose(uint32_t entityID, const VPMath::Vector3& P, const VPMath::Quaternion& Q);
	VPMath::Vector3 GetVelocity(uint32_t entityID);
	void AddVelocity(uint32_t entityID, const VPMath::Vector3& dir, float V);
	void SetVelocity(uint32_t entityID, const VPMath::Vector3& dir = {}, float V=0);
	VPMath::Vector3 GetGobalLocation(uint32_t entityID);
	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID);
	uint32_t FindIDByActor(physx::PxRigidActor* Actor);

	RaycastData RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance);
	RaycastData RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance);
	RaycastData RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
	RaycastData RaycastActorAtPose_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
	RaycastData RaycastActorAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
	std::vector<RaycastData> RaycastActorsAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
	std::vector<RaycastData> RaycastActorsAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance);
private:
	physx::PxRigidActor* FindActorByID(uint32_t entityID);
	void OnAddBodyScene(std::shared_ptr<RigidBody> rigidbody);
	void OnReleaseBodyScene(std::any data);
	void AddBodyScene(std::shared_ptr<RigidBody> body);

	std::weak_ptr<CollisionManager> m_Collsion;

	physx::PxPhysics* m_Physics = nullptr;
	physx::PxScene* m_Scene=nullptr;
	std::unordered_map<uint32_t, std::shared_ptr<RigidBody>> m_RigidBodyMap{};
	std::weak_ptr<PhysichResourceManager> m_ResourceManager;

};

