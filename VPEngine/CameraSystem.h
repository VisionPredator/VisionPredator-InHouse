#pragma once
#include "EventSubscriber.h"
#include "System.h"
class CameraSystem :
    public System, public IFixedUpdatable,public EventSubscriber
{
    CameraSystem(EntityManager* entityManager);
    ~CameraSystem() = default;

    void OnSetMainCamera(std::any data);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;

};

