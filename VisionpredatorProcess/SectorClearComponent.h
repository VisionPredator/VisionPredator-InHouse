#pragma once
#include <Component.h>
struct SectorClearComponent :
    public Component
{
    VP_JSONBODY(SectorClearComponent, DoorIdentity);
    std::string DoorIdentity{};


};

