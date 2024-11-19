#pragma once
#include <System.h>
class EditorSystem :
    public System,public IUpdatable,public IRenderable
{
public:
    EditorSystem(std::shared_ptr<SceneManager> sceneManager);
	~EditorSystem() = default;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;

private:
    bool isDebugRender = false;

    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;
};

