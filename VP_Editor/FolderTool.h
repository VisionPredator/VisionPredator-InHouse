#pragma once
#include "IImGui.h"

class FolderTool :
    public IImGui
{
public:
    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    FolderTool() = default;
};

