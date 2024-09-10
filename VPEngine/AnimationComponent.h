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

	float duration = 0;	//�ִϸ��̼� ����ð�
	float preDuration = 0;

	std::vector<std::wstring> animationList;

	bool isPlay = false;
	bool isChange = false;
	float speed = 1.0f;
	float transitionDuration = 0.2f;
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

