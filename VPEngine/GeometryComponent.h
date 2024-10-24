#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct GeometryComponent :
    public Component
{
	GeometryComponent();
	VP_JSONBODY(GeometryComponent, IsVisible, IsOverDraw, FBXFilter, color, UseTexture, TextureName)
		bool IsVisible = true; 
	bool IsOverDraw = false;

	GeoMetryFilter FBXFilter = GeoMetryFilter::Box;
	PassState pass = PassState::Geometry;
	VPMath::Color color{ 1,1,1};
	bool UseTexture = false;
	std::wstring  TextureName;
	std::shared_ptr<RenderData> Renderdata{};

};

