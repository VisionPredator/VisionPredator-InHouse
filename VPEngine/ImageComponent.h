#pragma once
#include "Component.h"
#include "../VPGraphics/vpGraphics.h"

struct ImageComponent : Component
{
	ImageComponent() = default;

	VP_JSONBODY(ImageComponent, RenderMode, Billboard, TexturePath, PosXPercent, PosYPercent, Scale, Layer, Color, LeftPercent, RightPercent, TopPercent, BottomPercent, isAlwaysOn);

	ui::RenderModeType RenderMode = ui::RenderModeType::ScreenSpaceOverlay;
	ui::BillboardType Billboard = ui::BillboardType::None;
	std::string TexturePath;
	float PosXPercent = 0.f;
	float PosYPercent = 0.f;
	float Scale = 1.f;
	uint32_t Layer = 1;		// �̹��� ���̰�. ���� ���� ���� �׷�����.
	VPMath::Color Color = { 1.f, 1.f, 1.f, 1.f };	// �̹��� ����. rgba
	float LeftPercent = 0.f; 
	float RightPercent = 0.f;
	float TopPercent = 0.f;
	float BottomPercent = 0.f;
	bool isAlwaysOn = false;
};

