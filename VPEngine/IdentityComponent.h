#pragma once
#include "Component.h"

struct IdentityComponent :
	public Component
{
	IdentityComponent();
	VP_JSONBODY(IdentityComponent, UUID)
		std::string UUID;
};

