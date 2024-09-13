#include "pch.h"
#include "PhysicModelLoader.h"
#include <iostream>

physx::PxConvexMesh* PhysicModelLoader::CookConvexMesh(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info)
{
    const std::vector<VPMath::Vector3>& vertices = info.Vertexs;

    // Check if vertices were successfully extracted
    if (vertices.empty()) {
        std::cout << "No vertices provided to cook convex mesh.\n";
        return nullptr; // Return null if no vertices were extracted
    }
    physx::PxConvexMeshDesc convexdesc;
    convexdesc.points.count = static_cast<physx::PxU32>(vertices.size());
    convexdesc.points.stride = sizeof(VPMath::Vector3);
    convexdesc.points.data = vertices.data();
    convexdesc.vertexLimit = 256; // Set the maximum number of vertices in the convex mesh
    convexdesc.polygonLimit = 1024; // Set the polygon limit based on the provided parameter
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
    // Handle cooking failure with detailed logging
    if (!status || result != physx::PxConvexMeshCookingResult::eSUCCESS) {
        switch (result) 
        {
            case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
                std::cout << "Convex mesh cooking failed: Zero area test failed.\n";
                break;
            case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
                std::cout << "Convex mesh cooking failed: Polygon limit reached.\n";
                break;
            case physx::PxConvexMeshCookingResult::eFAILURE:
                std::cout << "Convex mesh cooking failed: General failure.\n";
                break;
            default:
                std::cout << "Convex mesh cooking failed: Unknown error. Result code: " << static_cast<int>(result) << "\n";
                break;
        }
    }
    // Create the convex mesh from the cooked data
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = physics->createConvexMesh(input);

	return convexMesh;
}
