#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"

struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, FBX, animationList, speed,transitionDuration)
		AnimationComponent();

	std::wstring FBX = L"";

	int curAni = 0;
	int preAni = 0;

	float duration = 0;	//애니메이션 재생시간
	float preDuration = 0;

	std::vector<std::wstring> animationList;

	bool isPlay = false;
	bool isChange = false;
	float speed = 1.0f;
	float transitionDuration = 0.2f;
};


//fsm에서 바뀐 state에따라 바뀔거다
//
// 그러면 바뀐 상태에 따른 wstring을 넘겨준다 생각하면
// 상태에 따라 animationlist있는 wstring 넘겨준다../? index가 필요한디?
// 
// bool isLoop
//
// 상태에 따라 바뀔건데 어떻게 바꿔줄건데?
//

