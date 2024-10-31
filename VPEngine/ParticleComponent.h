#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, Restart, StartLifetimeA, StartLifetimeB, StartSizeA, StartSizeB, StartSpeedA, StartSpeedB, Duration);

	// TODO: Base �ؽ�ó Path ����ֱ�. �ƴϸ� ParticleObject �󿡼� null�̸� �б�Ÿ���� �ٲٱ�.
	std::string TexturePath;
	bool IsLoop = true;
	bool Restart = false;
	float StartLifetimeA = 1.f;	// ���� �ϳ��� ���ӽð�
	float StartLifetimeB = 1.f;
	float StartSizeA = 3.f;
	float StartSizeB = 3.f;
	float StartSpeedA = 5.f;
	float StartSpeedB = 5.f;

	bool RestartPrev = false;

	float Duration = 5.f; // ��ƼŬ �ý����� ��ƼŬ�� emit �ϴ� �ð��� ����
};
