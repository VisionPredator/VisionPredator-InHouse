#pragma once
#include "Component.h"
#include "NavMeshData.h"
struct NavAgentComponent :
    public Component
{
    VP_JSONBODY(NavAgentComponent, IsChase, Distance, TargetName)
        ~NavAgentComponent();
    std::string TargetName{};
    VPMath::Vector3 TargetLocation{};
    bool IsChase{};
    bool IsChanged{};
    float Distance{};
    std::shared_ptr<NavAgentData> NavAgent{};
};

