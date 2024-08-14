#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	VP_JSONBODY(MeshComponent, FBX, FBXFilter, LightMapOffset, LightMapScale, LightMapIndex)
		std::wstring FBX{};
	MeshFilter FBXFilter = MeshFilter::Static;
	std::shared_ptr<RenderData> Renderdata{};
	VPMath::Vector2 LightMapOffset{};
	float LightMapScale{};
	float LightMapIndex{};
};