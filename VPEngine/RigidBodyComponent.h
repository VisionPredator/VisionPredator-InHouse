#pragma once
#include "Component.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
struct RigidBodyComponent :
    public Component
{
    VP_JSONBODY(RigidBodyComponent, Test)
        RigidBodyComponent();
    VPMath::Vector3 Test;
    VPPhysics::EColliderType ShapeType;
    VPPhysics::BoxColliderInfo BoxInfo;
    VPPhysics::SphereColliderInfo SphereInfo;
    VPPhysics::CapsuleColliderInfo CapsuleBoxInfo;

};

