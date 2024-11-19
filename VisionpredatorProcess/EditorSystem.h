#pragma once
#include <System.h>
class EditorSystem :
    public System,public IUpdatable,public IRenderable
{
public:
    EditorSystem(std::shared_ptr<SceneManager> sceneManager);
	~EditorSystem() = default;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;

private:
    bool isDebugRender = false;

    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;
};

