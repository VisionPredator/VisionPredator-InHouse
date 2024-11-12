#pragma once
#include <Component.h>
struct PingComponent :
    public Component
{
    VP_JSONBODY(PingComponent, IsOn);
    bool IsOn=false;

};

