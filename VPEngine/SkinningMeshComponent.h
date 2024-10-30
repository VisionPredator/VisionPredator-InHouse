#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct SkinningMeshComponent :
	public Component
{
	SkinningMeshComponent() = default;
	VP_JSONBODY(SkinningMeshComponent, IsVisible, IsOverDraw, FBX, InteractiveColor)
		bool IsVisible = true;
	bool IsOverDraw = false;

	std::wstring FBX;
	std::shared_ptr<RenderData> Renderdata;
	VPMath::Color InteractiveColor = {1,0,0,1};

};

