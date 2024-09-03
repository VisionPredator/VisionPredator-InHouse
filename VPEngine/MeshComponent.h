#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent, FBX, pass, LightMapOffset,LightMapTiling, LightMapScale, LightMapIndex)
		std::wstring FBX{};
	std::shared_ptr<RenderData> Renderdata{};
	VPMath::Vector2 LightMapOffset{};
	VPMath::Vector2 LightMapTiling{};
	float LightMapScale{};
	float LightMapIndex{};
	PassState pass = PassState::Deferred;
};
