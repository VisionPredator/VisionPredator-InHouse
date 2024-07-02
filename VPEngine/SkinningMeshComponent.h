#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"

	struct SkinningMeshComponent :
		public Component
	{
		SkinningMeshComponent();
		VP_JSONBODY(SkinningMeshComponent, FBX)
		std::string FBX;
		MeshFilter FBXFilter = MeshFilter::Skinning;
	};

