#include "pch.h"
#include "PhysicModelLoader.h"

physx::PxConvexMesh* PhysicModelLoader::CookConvexMesh(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info)
{
    const std::vector<VPMath::Vector3>& vertices = info.Vertexs;

    // Check if vertices were successfully extracted
    if (vertices.empty()) {
        return nullptr; // Return null if no vertices were extracted
    }
    physx::PxConvexMeshDesc convexdesc;
    convexdesc.points.count = static_cast<physx::PxU32>(vertices.size());
    convexdesc.points.stride = sizeof(VPMath::Vector3);
    convexdesc.points.data = vertices.data();
    convexdesc.vertexLimit = 255; // Set the maximum number of vertices in the convex mesh
    convexdesc.polygonLimit = 64; // Set the polygon limit based on the provided parameter
    convexdesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX;

    // Set up cooking parameters
    physx::PxTolerancesScale scale;
    physx::PxCookingParams params(scale);
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
    params.buildGPUData = true;

    // Cook the convex mesh
    physx::PxDefaultMemoryOutputStream buf;
    physx::PxConvexMeshCookingResult::Enum result;
    bool status = PxCookConvexMesh(params, convexdesc, buf, &result);
    assert(status && result == physx::PxConvexMeshCookingResult::eSUCCESS);

    // Create the convex mesh from the cooked data
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = physics->createConvexMesh(input);

	return convexMesh;
}
