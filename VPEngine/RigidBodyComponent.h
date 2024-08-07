#pragma once
#include "Component.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
struct RigidBodyComponent :
    public Component
{
    VP_JSONBODY(RigidBodyComponent, IsDynamic, ColliderType, ColliderShape, BoxInfo, SphereInfo, CapsuleInfo, DefaultColliderInfo)
        RigidBodyComponent();
    bool IsDynamic = false;
    VPPhysics::EColliderType ColliderType{};
    VPPhysics::EColliderShape ColliderShape{};
    VPPhysics::BoxColliderInfo BoxInfo{};
    VPPhysics::SphereColliderInfo SphereInfo{};
    VPPhysics::CapsuleColliderInfo CapsuleInfo{};
    VPPhysics::ConvexColliderInfo ConvexColliderInfo{};
    VPPhysics::ColliderInfo DefaultColliderInfo{};

};

