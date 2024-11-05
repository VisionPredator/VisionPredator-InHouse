#pragma once
#include <System.h>
#include "EventSubscriber.h"
class WeaponBoxSystem :
    public System, public EventSubscriber,public IFixedUpdatable,public IStartable
{
public :
    WeaponBoxSystem(std::shared_ptr<SceneManager> scenemanager);

    void OnInterected(std::any interective_interector);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;

    // IStartable��(��) ���� ��ӵ�
    void Initialize() override;
    void Start(uint32_t gameObjectId) override;
    void Finish(uint32_t gameObjectId) override;
    void Finalize() override;
};

