#pragma once
#include "IPhysx.h"
class PhysxRigidManager;
class PhysxColliManager;
class Physics;
class CollisionCallback;
using namespace std;
class PhysxEngine :public Physic::IPhysx
{
public:
	PhysxEngine();
	~PhysxEngine();

	// IPhysx을(를) 통해 상속됨
	bool Initialize() override;
	bool Finalize() override;

private:
	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	PhysxRigidManager* m_PhysxRigidManager{};
	PhysxColliManager* m_PhysxCollisionManager{};
	CollisionCallback* m_Collisioncallback{};
	float m_UpdateTime = 1.f/ 60.f;
	float m_ElapsedTime = 0.f;

	// IPhysx을(를) 통해 상속됨
	void Update(float deltatime) override;

};

