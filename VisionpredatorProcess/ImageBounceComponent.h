#pragma once
#include <Component.h>
struct ImageBounceComponent :
	public Component
{
	VP_JSONBODY(ImageBounceComponent, AddScalePercent, MaxScalePercent, BouncingTime, BounceTimePercent)
		bool AddedBounce = false;
	bool Isbouncing = false;
	bool HasImage = false;
	float CommonScele{};
	float StartScale{};
	float EndScale{};
	float MaxScale{};
	float AddScale{};
	float AddScalePercent = 10.f;
	float MaxScalePercent = 25.f;
	float BouncingTime = 0.5f;
	float BounceProgress = 0.f;
	float BounceTimePercent = 10.f;
}
