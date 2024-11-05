#pragma once
#include "Component.h"
struct BulletComponent :public Component
{
	VP_JSONBODY(BulletComponent, SparkParticlePrefab, Damage, Speed)
	float Damage{};
	float Speed{};

	std::string SparkParticlePrefab{};
};

