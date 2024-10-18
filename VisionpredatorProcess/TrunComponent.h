#pragma once
#include "Component.h"
struct TrunComponent:public Component
{
	TrunComponent() = default;
	VP_JSONBODY(TrunComponent, Is_X, Is_Left, Angle, MoveTime)
	bool Is_X = true;
	float Angle;
	bool Is_Left{};
	VPMath::Quaternion StartPoint{};
	VPMath::Quaternion EndPoint{};
	float MoveTime{};
	float ProgressTime {};
	bool Finished{};
};

