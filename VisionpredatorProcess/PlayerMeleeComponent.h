#pragma once
#include <Component.h>
#include "VisPredStructs.h"
struct PlayerMeleeComponent :
	public Component
{
	PlayerMeleeComponent();
	VP_JSONBODY(PlayerMeleeComponent, DefalutPrefab, DashPrefab, SwordDamage, SwordLength, SwordAngle, VPDamage, VPLength, VPAngle)

		
		std::string DefalutPrefab{};
	std::string DashPrefab{};
	VisPred::Game::PlayerMelee AttackMode= VisPred::Game::PlayerMelee::Sword_First;
	float Length{};
	float Damage{};
	float Angle{};
	float Time{};
	bool IsLeft{};
	float SwordLength{};
	float VPLength{};
	float SwordDamage{};
	float VPDamage{};
	float SwordAngle{};
	float VPAngle{};



};

