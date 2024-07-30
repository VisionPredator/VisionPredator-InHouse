#pragma once
#include "VPPhysicsStructs.h"
class ConvexMeshResource
{
public:
    ConvexMeshResource(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info);
    ~ConvexMeshResource();
    inline physx::PxConvexMesh* GetConvexMesh();
    physx::PxConvexMesh* m_ConvexMesh;
};

inline physx::PxConvexMesh* ConvexMeshResource::GetConvexMesh()
{
    return m_ConvexMesh;
}