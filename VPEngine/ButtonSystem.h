#pragma once
#include "System.h"
class ButtonSystem :
    public System, public EventSubscriber, public IUpdatable, public IRenderable
{
public:
    ButtonSystem(std::shared_ptr<SceneManager> sceneManager);
    ~ButtonSystem() = default;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;


    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;

    void RenderUpdate(float deltaTime) override;

    void LateRenderUpdate(float deltaTime) override;

    void EditorRenderUpdate(float deltaTime) override;
private:

};

