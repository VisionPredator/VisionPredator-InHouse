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


	Physics* m_Physics{};
	physx::PxScene* m_PxScene{};
	PhysxRigidManager* m_PhysxRigidManager{};
	PhysxColliManager* m_PhysxCollisionManager{};
	CollisionCallback* m_Collisioncallback{};

	// IPhysx��(��) ���� ��ӵ�
	bool Initialize() override;
	bool Finalize() override;

};

