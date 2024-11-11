#pragma once
#include <Component.h>
struct TextBounceComponent :
    public Component
{
	VP_JSONBODY(TextBounceComponent, AddScalePercent, MaxScalePercent, BounceTimePercent,BouncingTime)

	bool AddedBounce = false;
	bool Isbouncing = false;
	bool HasText = false;
	float CommonScele{};
	float StartScale{};
	float EndScale{};
	float MaxScale{};
	float AddScale{};
	float AddScalePercent =10.f;
	float MaxScalePercent= 25.f;
	float BouncingTime =0.5f;
	float BounceProgress =0.f;
	float BounceTimePercent =10.f;
};

