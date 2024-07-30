#pragma once
#include "VPPhysicsStructs.h"
class ConvexMeshResource;
class ConvexMeshResourceManager
{
public:
	ConvexMeshResourceManager();
	bool Initialize(physx::PxPhysics* physics);
	~ConvexMeshResourceManager();

	std::shared_ptr<ConvexMeshResource> GetConvexMeshResource(const std::string& key);
	void LoadConvexMeshResource(const VPPhysics::ConvexMeshInfo& info);
	bool HasConvexMeshResource(const std::string& key);
	std::unordered_map<std::string, std::shared_ptr<ConvexMeshResource>> m_ConvexMeshMap;
	physx::PxPhysics* m_PxPhysics{};

};

