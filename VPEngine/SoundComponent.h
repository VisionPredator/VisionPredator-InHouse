#pragma once
#include "Component.h"

struct SoundComponent : public Component
{
	VP_JSONBODY(SoundComponent, SoundPath);
	std::string SoundPath{};
	float Duration{};
	bool Loop;
	float Volume{};
};

