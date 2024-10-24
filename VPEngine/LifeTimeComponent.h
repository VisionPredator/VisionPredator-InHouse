#pragma once
#include "Component.h"
struct LifeTimeComponent :
	public Component
{
	VP_JSONBODY(LifeTimeComponent, LifeTime);
	float LifeTime{};
};

