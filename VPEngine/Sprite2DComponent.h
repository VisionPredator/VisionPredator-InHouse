#pragma once
#include "Component.h"

struct Sprite2DComponent : Component
{
	Sprite2DComponent() = default;

	VP_JSONBODY(Sprite2DComponent, TexturePath, StartPosX, StartPosY, Width, Height, Layer, Color);

	std::string TexturePath;
	float StartPosX = 0.f;
	float StartPosY = 0.f;
	float Width = 1.f;
	float Height = 1.f;
	uint32_t Layer = 1;		// 이미지 깊이값. 높을 수록 위에 그려진다.
	VPMath::Vector4 Color = { 0.f, 0.f, 0.f, 1.f };	// 이미지 색상. rgba
};

