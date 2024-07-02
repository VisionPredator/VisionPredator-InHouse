#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
    struct MeshComponent :
        public Component
    {
        MeshComponent();
        VP_JSONBODY(MeshComponent, FBX, FBXFilter)
        std::string FBX;
        MeshFilter FBXFilter = MeshFilter::Static;

    };

