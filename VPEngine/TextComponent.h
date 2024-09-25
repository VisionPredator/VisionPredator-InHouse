#pragma once
#include "Component.h"

struct TextComponent : Component
{
	VP_JSONBODY(TextComponent, Text, Color, PosXPercent, PosYPercent, Scale, Layer);

	std::wstring Text = L"Text";
	VPMath::Color Color;
	float PosXPercent = 0.f;
	float PosYPercent = 0.f;
	float Scale = 1.f;
	uint32_t Layer = 0;

	// effect
	bool IsBold = false;
	bool IsItalic = false;
	bool IsStrikeout = false;
	bool IsUnderlined = false;
};

