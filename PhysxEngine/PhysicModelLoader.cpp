#include "pch.h"
#include "PhysicModelLoader.h"
#include <iostream>
#include "..\VPGraphics\Log.h"

physx::PxConvexMesh* PhysicModelLoader::CookConvexMesh(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info)
{
    const std::vector<VPMath::Vector3>& vertices = info.Vertexs;

    // 입력된 정점 데이터가 비어 있는 경우 처리
    if (vertices.empty()) 
    {
        Log::GetClientLogger()->error("Convex mesh cooking failed: No vertices provided.");
        return nullptr; // 정점 데이터가 없는 경우 nullptr 반환
    }

    // Convex Mesh 설명 생성
    physx::PxConvexMeshDesc convexdesc;
    convexdesc.points.count = static_cast<physx::PxU32>(vertices.size()); // 정점 개수 설정
    convexdesc.points.stride = sizeof(VPMath::Vector3); // 정점의 메모리 간격 설정
    convexdesc.points.data = vertices.data(); // 정점 데이터 지정
    convexdesc.vertexLimit = 256; // 정점 수 제한 설정
    convexdesc.polygonLimit = 1024; // 폴리곤 수 제한 설정
    convexdesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX; // Convex 생성 플래그 설정

    // Cooking 파라미터 설정
    physx::PxTolerancesScale scale;
    physx::PxCookingParams params(scale);
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH; // 메쉬 정리 비활성화
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE; // 활성 엣지 계산 비활성화
    params.buildGPUData = true; // GPU 데이터를 빌드하도록 설정

    // Convex Mesh 생성 (Cooking)
    physx::PxDefaultMemoryOutputStream buf;
    physx::PxConvexMeshCookingResult::Enum result;
    bool status = PxCookConvexMesh(params, convexdesc, buf, &result);

    // Cooking 실패 처리
    if (!status || result != physx::PxConvexMeshCookingResult::eSUCCESS) {
        switch (result) {
        case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
            Log::GetClientLogger()->error("Convex mesh cooking failed: Zero area test failed."); // 정점이 평면화된 경우
            break;
        case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
            Log::GetClientLogger()->error("Convex mesh cooking failed: Polygon limit reached."); // 폴리곤 수 제한 초과
            break;
        case physx::PxConvexMeshCookingResult::eFAILURE:
            Log::GetClientLogger()->error("Convex mesh cooking failed: General failure."); // 일반적인 실패
            break;
        default:
            const int resultCode = static_cast<int>(result);
            Log::GetClientLogger()->error("Convex mesh cooking failed: Unknown error. Result code: {}", resultCode); // 알 수 없는 실패
            break;
        }
        return nullptr; // 실패 시 nullptr 반환
    }

    // 생성된 Convex Mesh 데이터를 기반으로 Convex Mesh 생성
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = physics->createConvexMesh(input);

    return convexMesh; // 생성된 Convex Mesh 반환
}
