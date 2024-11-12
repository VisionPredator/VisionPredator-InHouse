#pragma once
#include <System.h>
#include "EventSubscriber.h"
struct PingSystem :
    public System,public IFixedUpdatable,public EventSubscriber
{
public: 
    PingSystem(std::shared_ptr<SceneManager> sceneManager);

    void OnInterected(std::any interective_interector);


    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
};

