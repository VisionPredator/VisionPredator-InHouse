#pragma once
#include <System.h>
#include "EventSubscriber.h"
class IdentityRemoverSystem :
    public System,public EventSubscriber
{
public:
    IdentityRemoverSystem(std::shared_ptr<SceneManager> scenemanager);

    void OnInterected(std::any entitys);
};

