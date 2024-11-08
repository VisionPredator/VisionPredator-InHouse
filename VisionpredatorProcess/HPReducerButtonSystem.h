#pragma once
#include <System.h>
#include "EventSubscriber.h"
class HPReducerButtonSystem :
    public System,public EventSubscriber
{
public:
    HPReducerButtonSystem(std::shared_ptr<SceneManager> sceneManager);
    void OnInterected(std::any data);
};

