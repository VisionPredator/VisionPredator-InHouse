#pragma once
#include "Component.h"
namespace VisPred
{
    struct MeshComponent :
        public Component
    {
        MeshComponent();
        VP_JSONBODY(MeshComponent, name)
        std::string name;
    };

}