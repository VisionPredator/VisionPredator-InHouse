#include "pch.h"
#include "PhysichResourceManager.h"
#include "ConvexMeshResource.h"
#include "VPPhysicsStructs.h"
PhysichResourceManager::PhysichResourceManager()
{
}

bool PhysichResourceManager::Initialize(physx::PxPhysics* physics)
{
	if (!physics) {
		return false;
	}
	m_PxPhysics = physics;
	return true;
}

PhysichResourceManager::~PhysichResourceManager()
{

}

std::shared_ptr<ConvexMeshResource> PhysichResourceManager::GetConvexMeshResource(const std::string& key)
{
	auto it = m_ConvexMeshMap.find(key);
	if (it == m_ConvexMeshMap.end())
		return nullptr;
	return it->second;
}
void PhysichResourceManager::LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info)
{
	std::shared_ptr<ConvexMeshResource> convexmesh = std::make_shared<ConvexMeshResource>(m_PxPhysics, info);
	m_ConvexMeshMap[info.FBXName] = convexmesh;
}

bool PhysichResourceManager::HasConvexMeshResource(const std::string& key)
{
	return m_ConvexMeshMap.find(key) != m_ConvexMeshMap.end();
}

