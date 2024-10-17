#pragma once
#include "Component.h"
struct AreaAttackComponent : public Component
{
	VP_JSONBODY(AreaAttackComponent, Damage);
	float Damage{};
};

