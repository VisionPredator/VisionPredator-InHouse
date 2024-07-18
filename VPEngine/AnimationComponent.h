#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, curAnimation, animationList, speed)
		AnimationComponent();

	std::wstring curAnimation = L"";
	std::wstring preAnimation = L"";

	float duration = 0.f;	//�ִϸ��̼� ����ð�
	float preDuration = 0.f;

	std::vector<std::wstring> animationList;

	bool isPlay = false;
	bool isChange = false;
	float speed = 1.0f;
};

