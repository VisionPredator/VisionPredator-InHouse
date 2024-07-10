#pragma once
#include "Component.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
struct RigidBodyComponent :
    public Component
{
    VP_JSONBODY(RigidBodyComponent, ColliderType, ColliderShape, BoxInfo, SphereInfo, CapsuleInfo, ColliderInfo)
        RigidBodyComponent();
    VPPhysics::EColliderType ColliderType{};
    VPPhysics::EColliderShape ColliderShape{};
    VPPhysics::BoxColliderInfo BoxInfo{};
    VPPhysics::SphereColliderInfo SphereInfo{};
    VPPhysics::CapsuleColliderInfo CapsuleInfo{};
    VPPhysics::ColliderInfo ColliderInfo{};

};

