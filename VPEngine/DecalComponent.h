#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"

class DecalComponent :
	public Component
{
public:
	DecalComponent() = default;
	VP_JSONBODY(DecalComponent, TextureName,WorldTransform);



	std::string TextureName{};
	VPMath::Matrix WorldTransform = {};

};
