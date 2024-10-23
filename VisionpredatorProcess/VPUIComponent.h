#pragma once
#include <Component.h>
struct VPUIComponent :
    public Component
{
    VP_JSONBODY(VPUIComponent, ChangeColor, FullImage, GageImage)
        VPMath::Color ChangeColor{};
    std::string FullImage{};
    std::string GageImage{};
};

