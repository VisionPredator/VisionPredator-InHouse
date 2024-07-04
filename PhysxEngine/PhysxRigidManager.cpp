#include "pch.h"
#include "PhysxRigidManager.h"

PhysxRigidManager::PhysxRigidManager()
{

}
PhysxRigidManager::~PhysxRigidManager()
{
	m_RigidBodies.clear();
}


bool PhysxRigidManager::Initialize(physx::PxPhysics* physics, PhysxColliManager* CollManager)
{
	m_Physics = physics;
	m_PhysxCollisionManager = CollManager;
	return true;
}

void PhysxRigidManager::Update()
{

}

