#pragma once
#include "System.h"
class SocketSystem :
    public System,public IUpdatable,public IRenderable
{
public:
    SocketSystem(std::shared_ptr<SceneManager> sceneManager);
    ~SocketSystem() = default;
    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;


    // IRenderable을(를) 통해 상속됨
    void EditorRenderUpdate(float deltaTime) override;

};

