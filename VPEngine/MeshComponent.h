#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct MeshComponent :
	public Component
{
	MeshComponent() = default;
	VP_JSONBODY(MeshComponent, FBX, FBXFilter, lightMapOffset, lightMapScale)
		std::wstring FBX;
	MeshFilter FBXFilter = MeshFilter::Static;
	std::shared_ptr<RenderData> Renderdata;
	float lightMapOffset;
	float lightMapScale;
}

