#pragma once
#include "Component.h"

struct Sprite2DComponent : Component
{
	Sprite2DComponent()
		: Color(VPMath::Color{1.f, 1.f, 1.f, 1.f})
	{
		
	}

	VP_JSONBODY(Sprite2DComponent, TexturePath, PosXPercent, PosYPercent, Scale, Layer, Color);

	std::string TexturePath;
	float PosXPercent = 0.f;
	float PosYPercent = 0.f;
	float Scale = 1.f;
	uint32_t Layer = 1;		// 이미지 깊이값. 높을 수록 위에 그려진다.
	VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };	// 이미지 색상. rgba
};

