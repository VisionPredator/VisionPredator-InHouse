#pragma once
#include "System.h"
#include "EventSubscriber.h"

class RenderSystem :
    public System, public EventSubscriber, public IRenderable
{
public:
    RenderSystem(std::shared_ptr<SceneManager> entityManager);
    ~RenderSystem() = default;
    void OnAddedComponent(std::any data);
    void OnReleasedComponent(std::any data);


    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;

    void MeshCompRender(MeshComponent& Comp);
    void SkincompRender(SkinningMeshComponent& Comp);
    void GeometryRender(GeometryComponent& Comp);


    // IRenderable을(를) 통해 상속됨
    void EditorRenderUpdate(float deltaTime) override;

};

