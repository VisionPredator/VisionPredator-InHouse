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

    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID;

    // ILateUpdatable��(��) ���� ��ӵ�
    void LateUpdate(float deltaTime) override;
};

