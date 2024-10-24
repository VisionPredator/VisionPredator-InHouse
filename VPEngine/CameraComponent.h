#pragma once
#include "Component.h"

struct CameraComponent :
	public Component
{
	VP_JSONBODY(CameraComponent, IsMain, NearZ, FarZ, FOV)
		CameraComponent();
	bool IsMain{ false };

	float NearZ = 1;
	float FarZ = 1000;
	float FOV =0.25f * 3.14f;
	VPMath::Matrix View;
	VPMath::Matrix Proj;
	VPMath::Matrix OrthoProj;
	float m_maxPitch = 89.f; // Convert degrees to radians
};


