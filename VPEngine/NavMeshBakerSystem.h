#pragma once
#include "System.h"
#include "NavStructs.h"


class NavMeshBakerSystem : public System, public IUpdatable,public IPhysicable,public IStartable
{
public:
    class NavMeshData;
    NavMeshBakerSystem(std::shared_ptr<SceneManager> sceneManager);
    ~NavMeshBakerSystem() = default;
    void MakeNavigationMesh(BuildSettings buildSettrings);
private:

    void makeNavMesh(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings = BuildSettings{});
    void AbleTest(std::vector<VPMath::Vector3> worldVertices, std::vector<int> faces, const BuildSettings& buildSettings = BuildSettings{})
    {
        static_assert(sizeof(VPMath::Vector3) == sizeof(float) * 3);
        assert(!worldVertices.empty() && !faces.empty());
        assert(faces.size() % 3 == 0);
        makeNavMesh(reinterpret_cast<float*>(&worldVertices[0]), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
    }



    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

    // IPhysicable을(를) 통해 상속됨
    void PhysicsUpdate(float deltaTime) override;

    // IStartable을(를) 통해 상속됨
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
    //friend NavigationAgent;
private:
    NavMeshData* m_NavMeshData{ nullptr };
};
class NavMeshBakerSystem::NavMeshData
{
private:
    NavMeshData(NavMeshBakerSystem* navFieldComponent) :navFieldComponent(navFieldComponent)
    {
        navQuery = dtAllocNavMeshQuery();
        crowd = dtAllocCrowd();
        context = std::make_unique<rcContext>(rcContext());
    }
    virtual ~NavMeshData()
    {
        rcFreePolyMesh(polyMesh);
        rcFreePolyMeshDetail(polyMeshDetail);
        dtFreeCrowd(crowd);
        dtFreeNavMeshQuery(navQuery);
        dtFreeNavMesh(navMesh);
    }
    friend NavMeshBakerSystem;
public:
    NavMeshBakerSystem* navFieldComponent;
    std::unique_ptr<rcContext> context;
    rcPolyMesh* polyMesh;
    rcConfig config;
    rcPolyMeshDetail* polyMeshDetail;
    class dtNavMesh* navMesh;
    class dtNavMeshQuery* navQuery;
    class dtCrowd* crowd;
};