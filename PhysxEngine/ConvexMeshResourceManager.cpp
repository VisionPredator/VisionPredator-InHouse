#include "pch.h"
#include "ConvexMeshResourceManager.h"
#include "ConvexMeshResource.h"
#include "VPPhysicsStructs.h"
ConvexMeshResourceManager::ConvexMeshResourceManager()
{
}

bool ConvexMeshResourceManager::Initialize(physx::PxPhysics* physics)
{
	if (!physics) {
		return false;
	}
	m_PxPhysics = physics;
	return true;
}

ConvexMeshResourceManager::~ConvexMeshResourceManager()
{

}

std::shared_ptr<ConvexMeshResource> ConvexMeshResourceManager::GetConvexMeshResource(const std::string& key)
{
	auto it = m_ConvexMeshMap.find(key);
	if (it == m_ConvexMeshMap.end())
		return nullptr;
	return it->second;
}
void ConvexMeshResourceManager::LoadConvexMeshResource(const VPPhysics::ConvexMeshInfo& info)
{
	std::shared_ptr<ConvexMeshResource> convexmesh = std::make_shared<ConvexMeshResource>(m_PxPhysics, info);
	m_ConvexMeshMap[info.FBXName] = convexmesh;
}

bool ConvexMeshResourceManager::HasConvexMeshResource(const std::string& key)
{
	return m_ConvexMeshMap.find(key) != m_ConvexMeshMap.end();
}

