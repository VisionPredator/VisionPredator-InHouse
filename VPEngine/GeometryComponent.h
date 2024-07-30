#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
struct GeometryComponent :
    public Component
{
	GeometryComponent();
	VP_JSONBODY(GeometryComponent, FBXFilter,pass,color,UseTexture, TextureName)
	MeshFilter FBXFilter = MeshFilter::Box;
	PassState pass = PassState::GeoMetry;
	VPMath::Color color{ 1,1,1 };
	bool UseTexture = false;
	std::string  TextureName;

};

