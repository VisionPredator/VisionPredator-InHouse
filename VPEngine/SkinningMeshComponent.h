#pragma once
#include "Component.h"
	struct SkinningMeshComponent :
		public Component
	{
		SkinningMeshComponent();

		VP_JSONBODY(SkinningMeshComponent, name)

		std::string name;

	};

