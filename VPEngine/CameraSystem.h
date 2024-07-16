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

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
    void CameraCalculation(CameraComponent& mainCamera);

    // IRenderable��(��) ���� ��ӵ�
    void RenderUpdate(float deltaTime) override;
    uint32_t m_MainCameraID;

    // IUpdatable��(��) ���� ��ӵ�
    void Update(float deltaTime) override;
};

