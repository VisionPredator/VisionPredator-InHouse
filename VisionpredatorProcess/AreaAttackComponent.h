#pragma once
#include "Component.h"
struct AreaAttackComponent : public Component
{
	AreaAttackComponent();

	VP_JSONBODY(AreaAttackComponent, Damage);
	float Damage{};
};

