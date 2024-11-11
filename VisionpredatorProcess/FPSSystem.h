#pragma once
#include <System.h>
class FPSSystem :
    public System,public IRenderable,public IUpdatable
{
public: 
    FPSSystem(std::shared_ptr<SceneManager> scenemanager);

    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
};

