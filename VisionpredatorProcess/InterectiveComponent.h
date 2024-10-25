#pragma once
#include <Component.h>
struct InterectiveComponent :
    public Component
{
    VP_JSONBODY(InterectiveComponent, IsInterective, SearcehdColor)
        bool IsInterective = true;
    VPMath::Color SearcehdColor = {1,0,0,1};
};

