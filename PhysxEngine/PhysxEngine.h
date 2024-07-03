#pragma once
#include "IPhysx.h"
class PhysxRigidManager;
class PhysxColliManager;
class Physics;

	using namespace std;
	class PhysxEngine :public IPhysx
{
public:
	PhysxEngine();
	~PhysxEngine();

	void Initialize();

	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	PhysxRigidManager* m_PhysxRigidManager{};
	PhysxColliManager* m_PhysxCollisionManager{};
};

