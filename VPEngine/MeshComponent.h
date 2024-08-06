#pragma once
#include "Component.h"
#include "../VPGraphics/MeshFilter.h"
    struct MeshComponent :
        public Component
    {
        MeshComponent() = default;
        VP_JSONBODY(MeshComponent, FBX, FBXFilter)
        std::wstring FBX;
        MeshFilter FBXFilter = MeshFilter::Static;
        //float a;
        //RenderData RenderData{};

    };

