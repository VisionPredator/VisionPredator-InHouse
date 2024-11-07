#pragma once
#include "Component.h"

class EnemySoundComponent : public Component
{
	EnemySoundComponent();

	int Volume_Run1{ 0 };
	int Volume_Run2{ 0 };
	int Volume_Hurt{ 0 };
	int Volume_Death{ 0 };
	int Volume_Attack{ 0 };

	std::string SoundKey_Run1{};
	std::string SoundKey_Run2{};
	std::string SoundKey_Hurt{};
	std::string SoundKey_Death{};
	std::string SoundKey_Attack{};
};

