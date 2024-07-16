#pragma once
#include "EventSubscriber.h"
#include "System.h"
class CameraSystem :
    public System, public IFixedUpdatable, public IRenderable,public EventSubscriber,public IUpdatable
{
public:
    CameraSystem(SceneManager* sceneManager);
    ~CameraSystem() = default;

    void OnSetMainCamera(std::any data);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
    void CameraCalculation(CameraComponent& mainCamera);

    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID;

    // IUpdatable을(를) 통해 상속됨
    void Update(float deltaTime) override;
};

