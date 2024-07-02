#pragma once
class Physics;

	using namespace std;
class PhysxEngine
{
public:
	PhysxEngine();
	~PhysxEngine();

	void Initialize();

	Physics* m_Physics;
	physx::PxScene* m_PxScene;
};

