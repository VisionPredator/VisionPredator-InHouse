#pragma once
#include "Component.h"
namespace VisPred
{

	struct IdentityComponent :
		public Component
	{
		IdentityComponent();
		VP_JSONBODY(IdentityComponent, UUID)
		std::string UUID;
	};
}

