#pragma once
#include "Component.h"

class EffectComponent :
    public Component
{
public:
	EffectComponent() = default;
	VP_JSONBODY(EffectComponent, isEffect, Duration, Speed, FBX, NoiseTexture, Offset, Scale, Rotation);

	bool isEffect = true;	
	float Duration = 0;
	float Speed = 0;

	std::wstring FBX{};
	std::wstring NoiseTexture{};

	VPMath::Vector3 Offset;
	VPMath::Vector3 Scale;
	VPMath::Vector3 Rotation;

};

