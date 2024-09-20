#pragma once
#include "Component.h"
#include "../VPGraphics/vpGraphics.h"

struct Sprite2DComponent : Component
{
	Sprite2DComponent()
		: Color(VPMath::Color{1.f, 1.f, 1.f, 1.f})
	{
		
	}

	VP_JSONBODY(Sprite2DComponent, RenderMode, TexturePath, PosXPercent, PosYPercent, Scale, Layer, Color);

	ui::RenderModeType RenderMode = ui::RenderModeType::ScreenSpaceOverlay;
	std::string TexturePath;
	float PosXPercent = 0.f;
	float PosYPercent = 0.f;
	float Scale = 1.f;
	uint32_t Layer = 1;		// �̹��� ���̰�. ���� ���� ���� �׷�����.
	VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };	// �̹��� ����. rgba
};

