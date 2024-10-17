#pragma once
#include <Component.h>
struct PlayerMeleeComponent :
	public Component
{
	PlayerMeleeComponent();
	VP_JSONBODY(PlayerMeleeComponent, SwordDamage, SwordLength, VPDamage, VPLength)
		VPMath::Vector2 SwordLength{};
		VPMath::Vector2 VPLength{};
	float SwordDamage{};
	float VPDamage{};



};

