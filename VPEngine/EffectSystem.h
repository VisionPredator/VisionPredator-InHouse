#pragma once
#include "System.h"
class EffectSystem :
	public System, public IRenderable, public ICompAddable, public IFixedUpdatable
{
public:
    EffectSystem(std::shared_ptr<SceneManager> sceneManager);
	~EffectSystem() = default;
    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;

    // ICompAddable��(��) ���� ��ӵ�
    void ComponentAdded(Component* comp) override;
    void ComponentReleased(Component* comp) override;


    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;

};

