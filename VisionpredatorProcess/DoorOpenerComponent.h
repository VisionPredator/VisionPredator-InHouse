#pragma once
#include "Component.h"
struct DoorOpenerComponent :public Component
{
	VP_JSONBODY(DoorOpenerComponent , Dummy);

	int Dummy{};
};

