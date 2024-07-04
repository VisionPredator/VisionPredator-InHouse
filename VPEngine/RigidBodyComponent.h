#pragma once
#include "Component.h"
struct RigidBodyComponent :
    public Component
{
    VP_JSONBODY(RigidBodyComponent, Test)
        RigidBodyComponent();
    VPMath::Vector3 Test;
};

