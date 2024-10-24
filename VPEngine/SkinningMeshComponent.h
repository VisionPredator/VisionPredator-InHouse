#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct SkinningMeshComponent :
	public Component
{
	SkinningMeshComponent() = default;
	VP_JSONBODY(SkinningMeshComponent, IsVisible, IsOverDraw, FBX)
		bool IsVisible = true;
	bool IsOverDraw = false;

		std::wstring FBX;
	std::shared_ptr<RenderData> Renderdata;

};

