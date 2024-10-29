#pragma once
#include "Component.h"

struct SoundComponent : public Component
{
	VP_JSONBODY(SoundComponent, SoundPath, Is2D,Loop, Volume);
	std::string SoundPath{};
	float Duration{};
	bool Is2D{};
	bool Loop;
	int Volume{};
};
