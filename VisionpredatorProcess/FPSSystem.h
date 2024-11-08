#pragma once
#include <System.h>
class FPSSystem :
    public System,public IRenderable,public IUpdatable
{
public: 
    FPSSystem(std::shared_ptr<SceneManager> scenemanager);

    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
};

