#pragma once
#include "IPhysx.h"
#include "EventSubscriber.h"
class RigidBodyManager;
class CollisionManager;
class ControllerManager;
class Physics;
class CollisionCallback;
class PhysichResourceManager;
using namespace std;
class PhysxEngine :public Physic::IPhysx, public EventSubscriber
{
public:
	PhysxEngine();
	~PhysxEngine();

	// IPhysx을(를) 통해 상속됨
	bool Initialize() override;
	bool Finalize() override;
	void Update(float deltatime) override;
	void SimulateUpdate(float deltatime);

	const VPPhysics::PhysicsInfo GetPhysicsInfo() { return m_PhyiscsInfo; }
	void  SetPhysicsInfo(VPPhysics::PhysicsInfo engineinfo) { m_PhyiscsInfo= engineinfo; }

	void OnSetPhysicInfo(std::any PhysicInfo);
	void ExtractVerticesAndFacesByLayer(EPhysicsLayer layer, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);
	void ExtractVerticesAndFaces(uint32_t entityID, std::vector<VPMath::Vector3>& outVertices, std::vector<int>& outIndices);

	void ConvertToStaticWithLayer(uint32_t entityID, EPhysicsLayer layer) override;
	void ConvertToDynamicWithLayer(uint32_t entityID, EPhysicsLayer layer) override;
	void ConvertToStatic(uint32_t entityID) override;
	void ConvertToDynamic(uint32_t entityID) override;
	void ReleaseActor(uint32_t entityID) override;

	void CreateStaticBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) override;
	void CreateStaticBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) override;
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) override;
	void CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) override;
	void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) override;
	VPMath::Vector3 GetGobalLocation(uint32_t entityID) override;
	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) override;
	void UpdatePhysicEngineInfo();
	void ApplyPhysicEngineInfo() override;

	// IPhysx을(를) 통해 상속됨
	void AddVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity) override;
	void SetVelocity(uint32_t entityID, VPMath::Vector3 Dir = {}, float velocity = {}) override;

	void AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power) override;

	// IPhysx을(를) 통해 상속됨
	VPMath::Vector3 GetVelocity(uint32_t entityID) override;

private:
	physx::PxScene* m_PxScene;
	std::shared_ptr<Physics> m_Physics;
	std::shared_ptr<RigidBodyManager> m_RigidBodyManager;
	std::shared_ptr<ControllerManager> m_ControllerManager;
	std::shared_ptr<CollisionManager> m_CollisionManager;
	std::shared_ptr<CollisionCallback> m_Collisioncallback;
	std::shared_ptr<PhysichResourceManager> m_RecourceManager;
	VPPhysics::PhysicsInfo m_PhyiscsInfo{};
	int m_Frame = 60;
	float m_UpdateTime = 1.f / 120.f;
	float m_ElapsedTime = 0.f;
	void CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo) override;
	void RemoveController(uint32_t entityID) override;
	void SetControllerGobalPose(uint32_t entityID, VPMath::Vector3 P) override;
	VPMath::Vector3 GetControllerGobalPose(uint32_t entityID) override;
	void SetControllerVelocity(uint32_t entityID, VPMath::Vector3 velocity) override;
	bool GetControllerIsFall(uint32_t entityID) override;
	void LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info) override;
	bool HasConvexMeshResource(const std::wstring& key) override;
	bool HasRigidBody(uint32_t entityID) override;
	bool HasController(uint32_t entityID) override;
	RaycastData RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance) override;
	RaycastData RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance) override;
	RaycastData RaycastActorAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	RaycastData RaycastActorAtPose_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	RaycastData RaycastActorAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	void ResizeCapsuleController(uint32_t entityID, float radius, float height) override;
	std::vector<RaycastData> RaycastActorsAtPose(VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	std::vector<RaycastData> RaycastActorsAtPose_Ignores(std::vector<uint32_t> entityIDs, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	bool IsDynamic(uint32_t entityID) override;
	bool IsStatic(uint32_t entityID) override;
	std::vector<VPMath::Vector3> GetConVexMeshVertex(uint32_t entityID) override;


	// IPhysx을(를) 통해 상속됨
	bool IsEntityInCollision(uint32_t entityID) override;

	bool IsEntityInTrigger(uint32_t entityID) override;

};

