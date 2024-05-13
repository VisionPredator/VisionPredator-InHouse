#pragma once
#include "Component.h"

namespace VisPred
{
	struct IDComponent : public Component
	{
		VP_JSONBODY(IDComponent, Name)
		IDComponent();
		std::string Name = "Entity";
	};
}