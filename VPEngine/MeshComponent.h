#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent, IsVisible, FBX, LightMapOffset, LightMapTiling, LightMapScale, LightMapIndex, MaskColor);

	std::shared_ptr<RenderData> Renderdata{};

	//serialize
	bool IsVisible = true;
	bool IsOverDraw = false;
	std::wstring FBX{};
	VPMath::Vector2 LightMapOffset{};
	VPMath::Vector2 LightMapTiling{};
	float LightMapScale{};
	float LightMapIndex{};
	VPMath::Color MaskColor;
};
