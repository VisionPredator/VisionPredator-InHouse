#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, curAnimation, animationList, speed)
		AnimationComponent();

	std::wstring curAnimation = L"";
	std::wstring preAnimation = L"";

	float duration = 0.f;	//애니메이션 재생시간

	std::vector<std::wstring> animationList;

	bool isPlay = false;
	float speed = 1.0f;
};

