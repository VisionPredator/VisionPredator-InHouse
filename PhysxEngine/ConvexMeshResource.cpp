#include "pch.h"
#include "ConvexMeshResource.h"
#include "PhysicModelLoader.h"


ConvexMeshResource::ConvexMeshResource(physx::PxPhysics* physics, const VPPhysics::ConvexMeshInfo& info)
{

	m_ConvexMesh = PhysicModelLoader::CookConvexMesh(physics, info);
	if (!m_ConvexMesh)
	{
		assert(false);
	}
}

inline ConvexMeshResource::~ConvexMeshResource()
{
	PX_RELEASE(m_ConvexMesh);
}