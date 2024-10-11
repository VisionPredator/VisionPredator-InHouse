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


	// IPhysx을(를) 통해 상속됨
	void CreateStaticBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) override;
	void CreateStaticBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) override;
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) override;
	void ReleaseActor(uint32_t entityID) override;
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo& boxinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::SphereColliderInfo& sphereinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo& capsuleinfo, const EColliderType& collidertype) override;
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
	//Physics* m_Physics{};
	//physx::PxScene* m_PxScene{};
	physx::PxScene* m_PxScene;
	std::shared_ptr<Physics> m_Physics;
	//RigidBodyManager* m_RigidBodyManager{};
	std::shared_ptr<RigidBodyManager> m_RigidBodyManager;
	std::shared_ptr<ControllerManager> m_ControllerManager;
	std::shared_ptr<CollisionManager> m_CollisionManager;
	std::shared_ptr<CollisionCallback> m_Collisioncallback;
	std::shared_ptr<PhysichResourceManager> m_RecourceManager;
	//ControllerManager* m_ControllerManager{};
	//CollisionManager* m_CollisionManager{};
	//CollisionCallback* m_Collisioncallback{};
	VPPhysics::PhysicsInfo m_PhyiscsInfo{};
	int m_Frame = 60;
	float m_UpdateTime = 1.f / 120.f;
	float m_ElapsedTime = 0.f;


	// IPhysx을(를) 통해 상속됨
	void CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo) override;
	void RemoveController(uint32_t entityID) override;
	void SetControllerGobalPose(uint32_t entityID, VPMath::Vector3 P) override;
	VPMath::Vector3 GetControllerGobalPose(uint32_t entityID) override;
	void SetControllerVelocity(uint32_t entityID, VPMath::Vector3 velocity) override;
	bool GetControllerIsFall(uint32_t entityID) override;
	void LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info) override;
	bool HasConvexMeshResource(const std::wstring& key) override;
	void CreateStaticBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) override;
	void CreateDynamicBody(const VPPhysics::ConvexColliderInfo& convexinfo, const EColliderType& collidertype) override;
	bool HasRigidBody(uint32_t entityID) override;
	bool HasController(uint32_t entityID) override;


	// IPhysx을(를) 통해 상속됨
	uint32_t RaycastToHitActor(uint32_t entityID, VPMath::Vector3 dir, float distance) override;
	uint32_t RaycastToHitActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance) override;
	uint32_t RaycastToHitActorFromLocation(VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;
	uint32_t RaycastToHitActorFromLocation_Ignore(uint32_t entityID, VPMath::Vector3 location, VPMath::Vector3 dir, float distance) override;





	// IPhysx을(를) 통해 상속됨
	void ResizeCapsuleController(uint32_t entityID, float radius, float height) override;




};

