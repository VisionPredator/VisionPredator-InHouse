#pragma once
#include "Component.h"
#include "NavMeshData.h"
struct NavAgentComponent :
    public Component
{
    VP_JSONBODY(NavAgentComponent, temp)
        ~NavAgentComponent();
    float temp=0.f;
    NavAgentData* NavAgnet{};
};

