#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, curAnimation, animeName1, animeName2, animeName3, animeName4, animeName5)
		AnimationComponent();

	std::wstring curAnimation = L"";
	std::wstring preAnimation = L"";

	float duration = 0.f;	//�ִϸ��̼� ����ð�

	 std::wstring animeName1;
	 std::wstring animeName2;
	 std::wstring animeName3;
	 std::wstring animeName4;
	 std::wstring animeName5;

};

