#pragma once
#include <Component.h>
#include "VisPredComponents.h"
struct AutoPickComponent :
	public Component
{
	VP_JSONBODY(AutoPickComponent, IsAuto)

		bool IsAuto{};
	std::list<uint32_t> PickUps{};
	PlayerComponent* ParentPlayer;

};

