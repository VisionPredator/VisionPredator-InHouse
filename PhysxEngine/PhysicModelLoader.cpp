#include "pch.h"
#include "PhysicModelLoader.h"
#include <iostream>
#include "..\VPGraphics\Log.h"

physx::PxConvexMesh* PhysicModelLoader::CookConvexMesh(physx::PxPhysics* physics, const VPPhysics::ConvexMeshResourceInfo& info)
{
    const std::vector<VPMath::Vector3>& vertices = info.Vertexs;

    // �Էµ� ���� �����Ͱ� ��� �ִ� ��� ó��
    if (vertices.empty()) 
    {
        Log::GetClientLogger()->error("Convex mesh cooking failed: No vertices provided.");
        return nullptr; // ���� �����Ͱ� ���� ��� nullptr ��ȯ
    }

    // Convex Mesh ���� ����
    physx::PxConvexMeshDesc convexdesc;
    convexdesc.points.count = static_cast<physx::PxU32>(vertices.size()); // ���� ���� ����
    convexdesc.points.stride = sizeof(VPMath::Vector3); // ������ �޸� ���� ����
    convexdesc.points.data = vertices.data(); // ���� ������ ����
    convexdesc.vertexLimit = 256; // ���� �� ���� ����
    convexdesc.polygonLimit = 1024; // ������ �� ���� ����
    convexdesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX; // Convex ���� �÷��� ����

    // Cooking �Ķ���� ����
    physx::PxTolerancesScale scale;
    physx::PxCookingParams params(scale);
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH; // �޽� ���� ��Ȱ��ȭ
    params.meshPreprocessParams |= physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE; // Ȱ�� ���� ��� ��Ȱ��ȭ
    params.buildGPUData = true; // GPU �����͸� �����ϵ��� ����

    // Convex Mesh ���� (Cooking)
    physx::PxDefaultMemoryOutputStream buf;
    physx::PxConvexMeshCookingResult::Enum result;
    bool status = PxCookConvexMesh(params, convexdesc, buf, &result);

    // Cooking ���� ó��
    if (!status || result != physx::PxConvexMeshCookingResult::eSUCCESS) {
        switch (result) {
        case physx::PxConvexMeshCookingResult::eZERO_AREA_TEST_FAILED:
            Log::GetClientLogger()->error("Convex mesh cooking failed: Zero area test failed."); // ������ ���ȭ�� ���
            break;
        case physx::PxConvexMeshCookingResult::ePOLYGONS_LIMIT_REACHED:
            Log::GetClientLogger()->error("Convex mesh cooking failed: Polygon limit reached."); // ������ �� ���� �ʰ�
            break;
        case physx::PxConvexMeshCookingResult::eFAILURE:
            Log::GetClientLogger()->error("Convex mesh cooking failed: General failure."); // �Ϲ����� ����
            break;
        default:
            const int resultCode = static_cast<int>(result);
            Log::GetClientLogger()->error("Convex mesh cooking failed: Unknown error. Result code: {}", resultCode); // �� �� ���� ����
            break;
        }
        return nullptr; // ���� �� nullptr ��ȯ
    }

    // ������ Convex Mesh �����͸� ������� Convex Mesh ����
    physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
    physx::PxConvexMesh* convexMesh = physics->createConvexMesh(input);

    return convexMesh; // ������ Convex Mesh ��ȯ
}
