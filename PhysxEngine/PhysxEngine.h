#pragma once
#include "IPhysx.h"
#include "../VPEngine/EventSubscriber.h"
class RigidBodyManager;
class CollisionManager;
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

	const VPPhysics::PhysicsInfo GetPhysicEngineInfo() { return m_EngineInfo; }
	void  SetPhysicEngineInfo(VPPhysics::PhysicsInfo engineinfo) { m_EngineInfo= engineinfo; }


private:
	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	RigidBodyManager* m_RigidManager{};
	CollisionManager* m_CollisionManager{};
	CollisionCallback* m_Collisioncallback{};
	VPPhysics::PhysicsInfo m_EngineInfo{};
	int m_Frame = 60;
	float m_UpdateTime = 1.f/ 120.f;
	float m_ElapsedTime = 0.f;
	// IPhysx을(를) 통해 상속됨


	// IPhysx을(를) 통해 상속됨
	void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) override;

	void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) override;

	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) override;


	// IPhysx을(를) 통해 상속됨
	void ReleaseActor(uint32_t entityID) override;


	// IPhysx을(를) 통해 상속됨
	void CreateDynamicBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) override;

	void CreateDynamicBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) override;

	void CreateDynamicBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) override;


	// IPhysx을(를) 통해 상속됨
	void SetGobalPose(uint32_t entityID, VPMath::Vector3 P, VPMath::Quaternion Q) override;

	VPMath::Vector3 GetGobalLocation(uint32_t entityID) override;

	VPMath::Quaternion GetGobalQuaternion(uint32_t entityID) override;

};

