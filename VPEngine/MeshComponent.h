#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent, FBX, FBXFilter, lightMapOffset, lightMapScale, lightMapIndex)
		std::wstring FBX{};
	MeshFilter FBXFilter = MeshFilter::Static;
	std::shared_ptr<RenderData> Renderdata{};
	VPMath::Vector2 lightMapOffset{};
	float lightMapScale{};
	float lightMapIndex{};
};
