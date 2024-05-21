#pragma once
#include "System.h"
#include "EventSubscriber.h"
class RenderSystem :
    public System, public EventSubscriber,public IFixedUpdatable,public IRenderable
{
public:
    RenderSystem(SceneManager* entityManager);
    ~RenderSystem() = default;
    void OnAddedComponent(std::any data);
    void OnReleasedComponent(std::any data);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;



};

