#pragma once
#include "Component.h"
struct BulletComponent :public Component
{
	VP_JSONBODY(BulletComponent, SparkParticleName, BloodSplashParticleName, Damage, Speed)
	int Damage{};
	float Speed{};

	std::string SparkParticleName{};
	std::string BloodSplashParticleName{};
};

