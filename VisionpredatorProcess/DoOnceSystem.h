#pragma once
#include <System.h>
#include "EventSubscriber.h"
#include "DoOnceResetComponent.h"
#include "DoOnceComponent.h"

class DoOnceSystem :
    public System,public EventSubscriber
{
public: 
    DoOnceSystem(std::shared_ptr<SceneManager> scenemanger);

    void OnInterected(std::any interective_interector);
    void DoOnceReset(DoOnceResetComponent* comp);
    void DoOnce(DoOnceComponent* comp);
};

