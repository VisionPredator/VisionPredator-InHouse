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




private:
	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	RigidBodyManager* m_RigidManager{};
	CollisionManager* m_CollisionManager{};
	CollisionCallback* m_Collisioncallback{};
	float m_UpdateTime = 1.f/ 60.f;
	float m_ElapsedTime = 0.f;
	VPPhysics::PhysicsInfo m_EngineInfo;
	// IPhysx을(를) 통해 상속됨


	// IPhysx을(를) 통해 상속됨
	void CreateStaticBody(const VPPhysics::BoxColliderInfo boxinfo, EColliderType collidertype) override;

	void CreateStaticBody(const VPPhysics::SphereColliderInfo sphereinfo, EColliderType collidertype) override;

	void CreateStaticBody(const VPPhysics::CapsuleColliderInfo capsuleinfo, EColliderType collidertype) override;


	// IPhysx을(를) 통해 상속됨
	void ReleaseActor(uint32_t entityID) override;

};

