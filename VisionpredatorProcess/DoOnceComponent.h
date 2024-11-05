#pragma once
#include <Component.h>
struct DoOnceComponent :
    public Component
{
    VP_JSONBODY(DoOnceComponent,temp)
    int temp{};
};

