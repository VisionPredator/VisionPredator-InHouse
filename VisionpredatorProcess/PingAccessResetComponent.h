#pragma once
#include <Component.h>
struct PingAccessResetComponent :
    public Component
{
    VP_JSONBODY(PingAccessResetComponent,  PingIdentity);
    std::vector <std::string> PingIdentity{};
};

