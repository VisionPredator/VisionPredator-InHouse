#pragma once
#include "VPPhysicsStructs.h"
class ConvexMeshResource;
class PhysichResourceManager
{
public:
	PhysichResourceManager();
	bool Initialize(physx::PxPhysics* physics);
	~PhysichResourceManager();

	std::shared_ptr<ConvexMeshResource> GetConvexMeshResource(const std::string& key);
	void LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info);
	bool HasConvexMeshResource(const std::string& key);
	std::unordered_map<std::string, std::shared_ptr<ConvexMeshResource>> m_ConvexMeshMap;
	physx::PxPhysics* m_PxPhysics{};

};
