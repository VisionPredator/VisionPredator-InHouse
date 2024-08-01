#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, curAnimation, animationList, speed)
		AnimationComponent();

	std::wstring curAnimation = L"";
	std::wstring preAnimation = L"";

	float duration = 0;	//애니메이션 재생시간
	float preDuration = 0;

	std::vector<std::wstring> animationList;

	bool isPlay = false;
	bool isChange = false;
	float speed = 1.0f;
};

