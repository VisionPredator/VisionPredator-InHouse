#pragma once
#include "Component.h"

struct IdentityComponent :
	public Component
{
	VP_JSONBODY(IdentityComponent, UUID)
		IdentityComponent();
	std::string UUID;
};

