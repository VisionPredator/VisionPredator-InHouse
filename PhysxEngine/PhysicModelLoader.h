#pragma once
#include "VPPhysicsStructs.h"
class PhysicModelLoader
{
public: 
	static physx::PxConvexMesh* CookConvexMesh(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info );

};

