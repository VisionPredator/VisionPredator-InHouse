#pragma once
#include <Component.h>

struct EnemySoundComponent : Component
{
	EnemySoundComponent();
	VP_JSONBODY(EnemySoundComponent,
		Volume_Run,
		Volume_Hurt,
		Volume_Attack,
		Volume_Death1,
		Volume_Death2,
		Volume_Death3,
		SoundKey_Run,
		SoundKey_Hurt,
		SoundKey_Attack,
		SoundKey_Death1,
		SoundKey_Death2,
		SoundKey_Death3
	)

	int Volume_Run{ 0 };
	int Volume_Hurt{ 0 };
	int Volume_Attack{ 0 };
	int Volume_Death1{ 0 };
	int Volume_Death2{ 0 };
	int Volume_Death3{ 0 };

	// TODO: 테스트 후 기본값 공백으로 되돌리기.
	std::string SoundKey_Run = "Enemy_Run";
	std::string SoundKey_Hurt = "Enemy_Hurt";
	std::string SoundKey_Attack{};
	std::string SoundKey_Death1 = "Enemy_Death1";
	std::string SoundKey_Death2 = "Enemy_Death2";
	std::string SoundKey_Death3 = "Enemy_Death3";
};

