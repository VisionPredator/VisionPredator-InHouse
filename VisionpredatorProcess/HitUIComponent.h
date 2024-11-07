#pragma once
#include "Component.h"
struct HitUIComponent : public Component
{
	VP_JSONBODY(HitUIComponent, DurationTime, ProgressTime);
	bool IsHitUIOn{};
	float DurationTime=1;
	float ProgressTime=0;
};

