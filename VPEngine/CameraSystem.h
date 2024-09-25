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

    // IRenderable��(��) ���� ��ӵ�
    void BeginRenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID=0;

    // ILateUpdatable��(��) ���� ��ӵ�
    void LateUpdate(float deltaTime) override;
private:
    void OnResize(std::any hwnd);
    float m_Width{};
    float m_Height{};

    // IRenderable��(��) ���� ��ӵ�
    void EditorRenderUpdate(float deltaTime) override;

    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;
    void LateRenderUpdate(float deltaTime) override;
};

