#pragma once
#include "EventSubscriber.h"
#include "System.h"
class CameraSystem :
    public System, public IFixedUpdatable,public EventSubscriber
{
    CameraSystem(EntityManager* entityManager);
    ~CameraSystem() = default;

    void OnSetMainCamera(std::any data);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;

};

