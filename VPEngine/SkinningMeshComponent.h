#pragma once
#include "Component.h"
	struct SkinningMeshComponent :
		public Component
	{
		SkinningMeshComponent();

		VP_JSONBODY(SkinningMeshComponent, Name)

		std::string Name;

	};

