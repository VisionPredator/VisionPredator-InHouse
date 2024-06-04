#pragma once
#include "Component.h"

struct CameraComponent :
	public Component
{
	VP_JSONBODY(CameraComponent, NearZ, FarZ, FOV, Ratio)
		CameraComponent();
	bool IsMain{ false };

	float NearZ = 0;
	float FarZ = 0;
	float FOV = 0;
	float Ratio = { 16.f / 9.f };	//16:9

	VPMath::Matrix View;
	VPMath::Matrix Proj;
};


