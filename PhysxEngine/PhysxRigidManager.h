#pragma once
class PhysxColliManager;
class RigidBody;
class PhysxRigidManager
{
public: 
	PhysxRigidManager();
	~PhysxRigidManager();
	bool Initialize(physx::PxPhysics* physics,	PhysxColliManager* CollManager);
	void Update();

private:
	physx::PxPhysics* m_Physics{};
	PhysxColliManager* m_PhysxCollisionManager{};
	std::unordered_map<uint32_t, RigidBody*> m_RigidBodies{};

};

