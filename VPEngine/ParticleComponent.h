#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop);

	// TODO: Base 텍스처 Path 집어넣기. 아니면 ParticleObject 상에서 null이면 분기타도록 바꾸기.
	std::string TexturePath;
	bool IsLoop = true;
};
