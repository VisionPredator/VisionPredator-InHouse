#pragma once
#include "System.h"
class MeshSystem :
    public System, public IRenderable, public ICompAddable
{
public:
    MeshSystem(std::shared_ptr<SceneManager> sceneManager);

    // ICompAddable��(��) ���� ��ӵ�
    void ComponentAdded(Component* comp) override;
    void ComponentReleased(Component* comp) override;

    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;
};

