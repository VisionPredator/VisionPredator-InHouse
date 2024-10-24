#pragma once
#include "System.h"
#include "EventSubscriber.h"

class RenderSystem :
    public System, public IRenderable,public ICompAddable
{
public:
    RenderSystem(std::shared_ptr<SceneManager> entityManager);
    ~RenderSystem() = default;
    // ICompAddable��(��) ���� ��ӵ�
    void ComponentAdded(Component* comp) override;

    void ComponentReleased(Component* comp) override;

    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;
    void OnButtonClick(std::any data);

};

