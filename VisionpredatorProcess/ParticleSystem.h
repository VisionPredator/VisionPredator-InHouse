#pragma once
#include <System.h>
#include "EventSubscriber.h"
class ParticleSystem :
    public System,public EventSubscriber,public IFixedUpdatable
{
public:
    ParticleSystem(std::shared_ptr <SceneManager> scenemanager);
    void OnFollowParticle(std::any entityid);

    // IFixedUpdatable��(��) ���� ��ӵ�
    void FixedUpdate(float deltaTime) override;
};

