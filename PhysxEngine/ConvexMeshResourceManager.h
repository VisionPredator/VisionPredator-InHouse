#pragma once
#include "VPPhysicsStructs.h"
class ConvexMesh;
class ConvexMeshResourceManager
{
public:
	ConvexMeshResourceManager();
	~ConvexMeshResourceManager();

	void CreatConvexMesh(VPPhysics::ConvexMeshInfo info);
	void GetConvexMesh();
	std::unordered_map<std::string, std::shared_ptr<ConvexMesh>> m_ConvexMeshMap;
};

