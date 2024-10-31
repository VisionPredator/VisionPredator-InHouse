#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, Restart, StartLifetime, StartSize, StartSpeed, Duration);

	// TODO: Base �ؽ�ó Path ����ֱ�. �ƴϸ� ParticleObject �󿡼� null�̸� �б�Ÿ���� �ٲٱ�.
	std::string TexturePath;
	bool IsLoop = true;
	bool Restart = false;
	VPMath::Vector2 StartLifetime = { 1.f, 1.f };
	VPMath::Vector2 StartSize = { 3.f, 3.f };
	VPMath::Vector2 StartSpeed = { 5.f, 5.f };

	bool RestartPrev = false;

	float Duration = 5.f; // ��ƼŬ �ý����� ��ƼŬ�� emit �ϴ� �ð��� ����
};
