#pragma once
#include "IImGui.h"
#include "../include/imgui.h"
class FolderTool :
    public IImGui
{
public:
    // IImGui��(��) ���� ��ӵ�
    void ImGuiRender() override;

    FolderTool() = default;
};

