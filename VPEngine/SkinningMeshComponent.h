#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct SkinningMeshComponent :
	public Component
{
	SkinningMeshComponent() = default;
	VP_JSONBODY(SkinningMeshComponent, FBX)
		std::wstring FBX;
	std::shared_ptr<RenderData> Renderdata;

};

