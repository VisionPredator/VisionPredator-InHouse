#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, Restart, StartLifetimeA, StartLifetimeB, StartSizeA, StartSizeB, Duration);

	// TODO: Base �ؽ�ó Path ����ֱ�. �ƴϸ� ParticleObject �󿡼� null�̸� �б�Ÿ���� �ٲٱ�.
	std::string TexturePath;
	bool IsLoop = true;
	bool Restart = false;
	float StartLifetimeA = 1.f;	// ���� �ϳ��� ���ӽð�
	float StartLifetimeB = 1.f;
	float StartSizeA = 3.f;
	float StartSizeB = 3.f;

	bool RestartPrev = false;

	float Duration = 5.f; // ��ƼŬ �ý����� ��ƼŬ�� emit �ϴ� �ð��� ����
};
