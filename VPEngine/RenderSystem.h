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

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;



};

