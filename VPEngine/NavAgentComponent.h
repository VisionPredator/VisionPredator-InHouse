#pragma once
#include "Component.h"
#include "NavMeshData.h"
struct NavAgentComponent :
    public Component
{
    VP_JSONBODY(NavAgentComponent, TargetName)
        ~NavAgentComponent();
    std::string TargetName{};
    bool IsChase{};
    NavAgentData* NavAgent{};
};

