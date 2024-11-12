#pragma once
#include <Component.h>
struct ScoreComponent :
	public Component
{
	VP_JSONBODY(ScoreComponent, Ment, EndMent, FontColor, EndFontColor, MiddlePose, MiddleScale);
	int PlayerPoint = 0;
	std::wstring Ment = L"Á¡¼ö : ";
	std::wstring EndMent = L"My Score : ";
	VPMath::Color FontColor{};
	VPMath::Color EndFontColor{};
	VPMath::Vector2 MiddlePose{};
	float MiddleScale{};
};

