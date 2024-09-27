#pragma once
#include "System.h"
class UISystem :
    public System, public IRenderable,public ICompAddable
{
public:
    UISystem(std::shared_ptr<SceneManager> sceneManager);
    ~UISystem() = default;

    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;

    // ICompAddable을(를) 통해 상속됨
    void ComponentAdded(Component* comp) override;
    void ComponentReleased(Component* comp) override;
};

