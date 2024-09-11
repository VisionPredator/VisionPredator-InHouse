#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent,Pass, FBX, LightMapOffset,LightMapTiling, LightMapScale, LightMapIndex)

	std::shared_ptr<RenderData> Renderdata{};
	PassState Pass = PassState::Deferred;

	//serialize
	std::wstring FBX{};
	VPMath::Vector2 LightMapOffset{};
	VPMath::Vector2 LightMapTiling{};
	float LightMapScale{};
	float LightMapIndex{};
};
