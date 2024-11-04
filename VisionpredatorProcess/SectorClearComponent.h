#pragma once
#include <Component.h>
struct SectorClearComponent :
    public Component
{
    VP_JSONBODY(SectorClearComponent, OpenDoorIdentity, PreDoorIdentity);
    std::vector < std::string > OpenDoorIdentity{};
    std::string PreDoorIdentity{};

};

