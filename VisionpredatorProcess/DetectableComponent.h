#pragma once
#include <Component.h>
struct DetectableComponent :
    public Component
{
    VP_JSONBODY(DetectableComponent, IsDectable)
    bool IsDectable{};
    VPMath::Color DetectedColor = {1,0,0,1};
};

