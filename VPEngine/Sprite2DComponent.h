#pragma once
#include "Component.h"

struct Sprite2DComponent : Component
{
	Sprite2DComponent()
		: Color(VPMath::Color{1.f, 1.f, 1.f, 1.f})
	{
		
	}

	VP_JSONBODY(Sprite2DComponent, TexturePath, StartPosX, StartPosY, Scale, Layer, Color);

	std::string TexturePath;
	float StartPosX = 0.f;
	float StartPosY = 0.f;
	float Scale = 1.f;
	uint32_t Layer = 1;		// �̹��� ���̰�. ���� ���� ���� �׷�����.
	VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };	// �̹��� ����. rgba
};

