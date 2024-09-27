#pragma once
#include "EventSubscriber.h"
#include "System.h"
class CameraSystem :
    public System, public IRenderable,public EventSubscriber,public ILateUpdatable
{
public:
    CameraSystem(std::shared_ptr<SceneManager> sceneManager);
    ~CameraSystem() = default;

    void OnSetMainCamera(std::any data);
    void CameraCalculation(CameraComponent& mainCamera);

    // IRenderable을(를) 통해 상속됨
    void BeginRenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID=0;

    // ILateUpdatable을(를) 통해 상속됨
    void LateUpdate(float deltaTime) override;
private:
    void OnResize(std::any hwnd);
    float m_Width{};
    float m_Height{};

    // IRenderable을(를) 통해 상속됨
    void EditorRenderUpdate(float deltaTime) override;

    // IRenderable을(를) 통해 상속됨
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
};

