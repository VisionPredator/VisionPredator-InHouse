#pragma once
#include "CommonInclude.h"
#include "VPPhysicsStructs.h"
class ConvexMeshResource;
class PhysichResourceManager
{
public:
	PhysichResourceManager();
	bool Initialize(physx::PxPhysics* physics);
	~PhysichResourceManager();

	std::shared_ptr<ConvexMeshResource> GetConvexMeshResource(const std::wstring& key);
	void LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info);
	bool HasConvexMeshResource(const std::wstring& key);
	std::unordered_map<std::wstring, std::shared_ptr<ConvexMeshResource>> m_ConvexMeshMap;
	physx::PxPhysics* m_PxPhysics{};

};

