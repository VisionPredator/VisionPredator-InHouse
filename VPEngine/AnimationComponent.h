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

	float duration = 0;	//�ִϸ��̼� ����ð�
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


//fsm���� �ٲ� state������ �ٲ�Ŵ�
//
// �׷��� �ٲ� ���¿� ���� wstring�� �Ѱ��ش� �����ϸ�
// ���¿� ���� animationlist�ִ� wstring �Ѱ��ش�../? index�� �ʿ��ѵ�?
// 
// bool isLoop
//
// ���¿� ���� �ٲ�ǵ� ��� �ٲ��ٰǵ�?
//

