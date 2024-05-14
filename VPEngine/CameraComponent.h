#pragma once
#include "Component.h"

struct CameraComponent :
	public Component
{
	VP_JSONBODY(CameraComponent, testMatrix)
		CameraComponent();
	VPMath::Matrix testMatrix{};
	bool IsMain{ false };
};


