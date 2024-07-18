#pragma once
#include "System.h"
#include "EventSubscriber.h"

class RenderSystem :
    public System, public EventSubscriber, public IRenderable
{
public:
    RenderSystem(SceneManager* entityManager);
    ~RenderSystem() = default;
    void OnAddedComponent(std::any data);
    void OnReleasedComponent(std::any data);


    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;

    void MeshCompRender(MeshComponent& Comp);
    void SkincompRender(SkinningMeshComponent& Comp);

    std::jthread* m_Thread1;
    std::jthread* m_Thread2;
};

