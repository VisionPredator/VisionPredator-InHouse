#pragma once
#include <Component.h>
struct PlayerComponent;
struct AutoPickComponent :
	public Component
{
	VP_JSONBODY(AutoPickComponent, IsAuto)

		bool IsAuto{};
	std::list<uint32_t> PickUps{};
	PlayerComponent* ParentPlayer;

};

