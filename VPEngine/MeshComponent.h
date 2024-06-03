#pragma once
#include "Component.h"
    struct MeshComponent :
        public Component
    {
        MeshComponent();
        VP_JSONBODY(MeshComponent, name)
        std::string name;
        std::string VS;
        std::string PS;
        std::string FBX;
        






    };

