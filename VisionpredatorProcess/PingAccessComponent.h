#pragma once
#include <Component.h>
struct PingAccessComponent :
    public Component
{
    VP_JSONBODY(PingAccessComponent, IsUsed, PingIdentity);
    bool IsUsed = false;
    std::vector <std::string> PingIdentity{};
};

