#pragma once
#include "Component.h"
struct AreaAttackComponent : public Component
{
	AreaAttackComponent();

	VP_JSONBODY(AreaAttackComponent, IdentityAttach, Damage);
	int Damage{};
	std::string IdentityAttach{};
	std::weak_ptr<Entity> AttachedEntity{};
};

