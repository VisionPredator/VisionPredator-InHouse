#pragma once
#include "Component.h"
namespace VisPred
{
	struct SkinningMeshComponent :
		public Component
	{
		SkinningMeshComponent();

		VP_JSONBODY(SkinningMeshComponent, name)

		std::string name;

	};
}

