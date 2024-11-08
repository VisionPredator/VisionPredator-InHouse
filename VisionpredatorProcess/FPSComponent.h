#pragma once
#include <Component.h>
struct FPSComponent :
    public Component
{
    VP_JSONBODY(FPSComponent, IsShow, Time);
    bool IsShow{};
    float Time{};
};

