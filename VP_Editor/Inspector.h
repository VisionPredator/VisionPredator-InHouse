#pragma once
#include "IImGui.h"
class Inspector :
    public IImGui
{
public:
    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Inspector() = default;
};

