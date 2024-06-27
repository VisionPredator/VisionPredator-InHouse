#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct LightComponent :
    public Component
{
	VP_JSONBODY(LightComponent, type, intensity, color, direction, attenuation, range, spot)
		LightComponent();

	LightType type = LightType::Direction;
	float intensity{1};
	VPMath::Color color{1,1,1};
	VPMath::Vector3 direction{1,0,0};
	VPMath::Vector3 attenuation{};
	float range{-1};
	float spot{-1};

};

