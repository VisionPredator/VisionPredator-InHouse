#pragma once
#include <Component.h>
struct SectorClearComponent :
    public Component
{
    VP_JSONBODY(SectorClearComponent, OpenDoorIdentity);
    std::vector < std::string > OpenDoorIdentity{};

};

