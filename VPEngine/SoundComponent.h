#pragma once
#include "Component.h"

struct SoundComponent : public Component
{
	VP_JSONBODY(SoundComponent, SoundPath, Loop, Volume);
	std::string SoundPath{};
	float Duration{};
	bool Loop;
	float Volume{};
};

