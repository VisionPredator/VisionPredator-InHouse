#pragma once
#include <Component.h>
struct FollowComponent :
    public Component
{
    VP_JSONBODY(FollowComponent, Isfollowing)
    bool Isfollowing{};
    uint32_t FollowingEntityID{};
    int temp{};
};

