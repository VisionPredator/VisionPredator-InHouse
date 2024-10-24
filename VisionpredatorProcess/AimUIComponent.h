#pragma once
#include <Component.h>
struct AimUIComponent :
	public Component
{
	VP_JSONBODY(AimUIComponent
		, Killed
		, Aimed
		, Attacked
		, Interected);
	std::string Killed{};
	std::string Aimed{};
	std::string Attacked{};
	std::string Interected{};
};

