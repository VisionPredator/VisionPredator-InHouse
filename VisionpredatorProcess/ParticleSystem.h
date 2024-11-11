#pragma once
#include <System.h>
#include "EventSubscriber.h"
class ParticleSystem :
    public System,public EventSubscriber,public IFixedUpdatable
{
public:
    ParticleSystem(std::shared_ptr <SceneManager> scenemanager);
    void OnFollowParticle(std::any entityid);

    // IFixedUpdatable을(를) 통해 상속됨
    void FixedUpdate(float deltaTime) override;
};

