#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct LightComponent :
    public Component
{
	VP_JSONBODY(LightComponent, type, intensity, color, direction, attenuation, range, spot)
		LightComponent();

	LightType type{};
	float intensity;
	VPMath::Color color;
	VPMath::Vector3 direction;
	VPMath::Vector3 attenuation;
	float range;
	float spot;
};

