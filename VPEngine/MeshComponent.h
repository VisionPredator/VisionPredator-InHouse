#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent, FBX, LightMapOffset,LightMapTiling, LightMapScale, LightMapIndex)

	std::shared_ptr<RenderData> Renderdata{};

	//serialize
	std::wstring FBX{};
	VPMath::Vector2 LightMapOffset{};
	VPMath::Vector2 LightMapTiling{};
	float LightMapScale{};
	float LightMapIndex{};
};
