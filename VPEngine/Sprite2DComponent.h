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
	uint32_t Layer = 1;		// �̹��� ���̰�. ���� ���� ���� �׷�����.
	VPMath::Vector4 Color = { 0.f, 0.f, 0.f, 1.f };	// �̹��� ����. rgba
};

