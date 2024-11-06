#pragma once
#include "Component.h"
struct BulletComponent :public Component
{
	VP_JSONBODY(BulletComponent, SparkParticleName, Damage, Speed)
	float Damage{};
	float Speed{};

	std::string SparkParticleName{};
};

