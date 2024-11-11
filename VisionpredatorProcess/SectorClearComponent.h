#pragma once
#include <Component.h>
struct SectorClearComponent :
    public Component
{
    VP_JSONBODY(SectorClearComponent, OpenDoorIdentity, Ment);
    std::vector < std::string > OpenDoorIdentity{};
    std::wstring Ment = L"³²Àº Àû : ";
};

