#pragma once
#include "EventSubscriber.h"
#include "System.h"
class CameraSystem :
    public System, public IRenderable,public EventSubscriber,public ILateUpdatable
{
public:
    CameraSystem(SceneManager* sceneManager);
    ~CameraSystem() = default;

    void OnSetMainCamera(std::any data);
    void CameraCalculation(CameraComponent& mainCamera);

    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID;

    // ILateUpdatable을(를) 통해 상속됨
    void LateUpdate(float deltaTime) override;
};

