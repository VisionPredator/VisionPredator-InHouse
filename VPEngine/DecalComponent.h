#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"

class DecalComponent :
	public Component
{
public:
	DecalComponent() = default;
	VP_JSONBODY(DecalComponent, TextureName,VolumeSize);



	std::string TextureName{};
	VPMath::Vector3 VolumeSize{1,1,1};
};
