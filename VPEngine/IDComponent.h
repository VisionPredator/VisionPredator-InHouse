#pragma once
#include "Component.h"

struct IDComponent : public Component
{
	VP_JSONBODY(IDComponent, Name)
		IDComponent();
	std::string Name = "Entity";
};
