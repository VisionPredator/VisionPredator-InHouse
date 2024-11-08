#pragma once
#include "System.h"
class EffectSystem :
	public System, public IRenderable, public ICompAddable, public IFixedUpdatable
{
public:
    EffectSystem(std::shared_ptr<SceneManager> sceneManager);
	~EffectSystem() = default;
    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
    void EditorRenderUpdate(float deltaTime) override;

    // ICompAddable을(를) 통해 상속됨
    void ComponentAdded(Component* comp) override;
    void ComponentReleased(Component* comp) override;


    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;

};

