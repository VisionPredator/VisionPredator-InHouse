#pragma once
#include "System.h"
#include "NavStructs.h"
#include "NavMeshData.h"
class NavMeshSystem : public System,public IPhysicable,public IStartable,public IRenderable,public IFixedUpdatable
{
public:
    NavMeshSystem(std::shared_ptr<SceneManager> sceneManager);
    ~NavMeshSystem() = default;
    void MakeNavigationMesh(BuildSettings buildSettrings);
public:

    void makeNavMesh(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const BuildSettings& buildSettings = BuildSettings{});
    void AbleTest(std::vector<VPMath::Vector3> worldVertices, std::vector<int> faces, const BuildSettings& buildSettings = BuildSettings{})
    {
        static_assert(sizeof(VPMath::Vector3) == sizeof(float) * 3);
        assert(!worldVertices.empty() && !faces.empty());
        assert(faces.size() % 3 == 0);
        makeNavMesh(reinterpret_cast<float*>(&worldVertices[0]), worldVertices.size(), &faces[0], faces.size() / 3, buildSettings);
    }

    void FixedUpdate(float deltaTime) override;

    // IPhysicable��(��) ���� ��ӵ�
    void PhysicsUpdate(float deltaTime) override;

    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
    
private:
    friend NavMeshData;
    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;

    // IRenderable��(��) ���� ��ӵ�
    void EditorRenderUpdate(float deltaTime) override;
};

