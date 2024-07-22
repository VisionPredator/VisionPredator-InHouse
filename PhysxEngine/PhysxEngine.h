#pragma once
#include "IPhysx.h"
#include "../VPEngine/EventSubscriber.h"
class RigidBodyManager;
class CollisionManager;
class ControllerManager;
class Physics;
class CollisionCallback;
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

	const VPPhysics::PhysicsInfo GetPhysicsInfo() { return m_PhyiscsInfo; }
	void  SetPhysicsInfo(VPPhysics::PhysicsInfo engineinfo) { m_PhyiscsInfo= engineinfo; }

	void OnSetPhysicInfo(std::any PhysicInfo);

	// IPhysx을(를) 통해 상속됨


	// IPhysx을(를) 통해 상속됨
	void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) override;
	void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) override;
	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) override;
	void ReleaseActor(uint32_t entityID) override;
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) override;
	void CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) override;
	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) override;
	void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) override;
	VPMath::Vector3 GetGobalLocation(uint32_t entityID) override;
	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) override;
	void UpdatePhysicEngineInfo();
	void ApplyPhysicEngineInfo() override;

	// IPhysx을(를) 통해 상속됨
	void AddVelocity(uint32_t entityID, VPMath::Vector3 Dir, float velocity) override;

	void AddImpulse(uint32_t entityID, VPMath::Vector3 Dir, float power) override;

	// IPhysx을(를) 통해 상속됨
	VPMath::Vector3 GetVelocity(uint32_t entityID) override;

private:
	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	RigidBodyManager* m_RigidBodyManager{};
	ControllerManager* m_ControllerManager{};
	CollisionManager* m_CollisionManager{};
	CollisionCallback* m_Collisioncallback{};
	VPPhysics::PhysicsInfo m_PhyiscsInfo{};
	int m_Frame = 60;
	float m_UpdateTime = 1.f / 120.f;
	float m_ElapsedTime = 0.f;


	// IPhysx을(를) 통해 상속됨
	void CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo, VPPhysics::PhysicsInfo physicsinfo) override;


	// IPhysx을(를) 통해 상속됨
	void RemoveController(uint32_t entityID) override;

};

