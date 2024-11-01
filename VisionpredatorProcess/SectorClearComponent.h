#pragma once
#include <Component.h>
struct SectorClearComponent :
    public Component
{
    VP_JSONBODY(SectorClearComponent, DoorIdentity, PreDoorIdentity);
    std::string DoorIdentity{};
    std::string PreDoorIdentity{};

};

