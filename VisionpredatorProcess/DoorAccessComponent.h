#pragma once
#include "Component.h"
struct DoorAccessComponent:
    public Component
{
    VP_JSONBODY(DoorAccessComponent, Open, DoorIdentitys);
    bool Open{};
	std::vector <std::string> DoorIdentitys{};
};

