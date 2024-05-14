#pragma once
#include "Component.h"
 


struct CameraComponent :
    public Component
{
    CameraComponent();
    VPMath::Matrix testMatrix{};
    bool IsMain{false};


};


