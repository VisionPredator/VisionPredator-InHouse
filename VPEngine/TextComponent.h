#pragma once
#include "Component.h"

struct TextComponent : Component
{
	VP_JSONBODY(TextComponent, Text, FontPath, Color, PosXPercent, PosYPercent, Scale, Angle, Layer);

	std::wstring Text = L"한국어";
	std::wstring FontPath = {};
	VPMath::Color Color;
	float PosXPercent = 0.f;
	float PosYPercent = 0.f;
	float Scale = 1.f;
	float Angle = 0.f; // 0도 ~ 360도
	float Layer = 0.f;

	// effect
	bool IsBold = false;
	bool IsItalic = false;
	bool IsStrikeout = false;
	bool IsUnderlined = false;
};

