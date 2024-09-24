#pragma once
#include "System.h"
class SocketSystem :
    public System,public IUpdatable,public IRenderable
{
public:
    SocketSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SocketSystem() = default;
    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;


    // IRenderable��(��) ���� ��ӵ�
    void EditorRenderUpdate(float deltaTime) override;

};

