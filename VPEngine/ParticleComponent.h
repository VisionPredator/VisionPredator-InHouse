#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop, StartLifetimeA, StartLifetimeB, StartSizeA, StartSizeB, Duration);

	// TODO: Base 텍스처 Path 집어넣기. 아니면 ParticleObject 상에서 null이면 분기타도록 바꾸기.
	std::string TexturePath;
	bool IsLoop = true;
	float StartLifetimeA = 1.f;	// 입자 하나의 지속시간
	float StartLifetimeB = 1.f;
	float StartSizeA = 3.f;
	float StartSizeB = 3.f;

	float Duration = 5.f; // 파티클 시스템이 파티클을 emit 하는 시간의 길이
};
