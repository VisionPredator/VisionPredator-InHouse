#pragma once
#include "Component.h"

struct ParticleComponent : Component
{
	ParticleComponent() = default;

	VP_JSONBODY(ParticleComponent, TexturePath, IsLoop);

	// TODO: Base �ؽ�ó Path ����ֱ�. �ƴϸ� ParticleObject �󿡼� null�̸� �б�Ÿ���� �ٲٱ�.
	std::string TexturePath;
	bool IsLoop = true;
};
