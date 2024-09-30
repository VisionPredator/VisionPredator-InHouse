#pragma once
#include "Component.h"
#include "..\VPGraphics\CBuffer.h"
#include "..\VisionpredatorProcess\VisPredStructs.h"
struct AnimationComponent : public Component
{
	VP_JSONBODY(AnimationComponent, FBX, animationList, curAni, speed, isLoop, transitionDuration)
		AnimationComponent();

	std::wstring FBX = L"";

	int curAni = 1;	
	int preAni = 1;	

	float duration = 0;	//애니메이션 재생시간
	float preDuration = 0;

	std::vector<std::wstring> animationList;
	 
	bool isPlay = true;
	bool isLoop = true;
	bool IsFinished = false;
	bool IsBlending = false;
	float speed = 1.0f;
	float transitionDuration = 0.2f;
	VisPred::Game::PlayerAni PlayerCurAni{};
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

