#pragma once
#include "IImGui.h"

class FolderTool :
    public IImGui
{
public:
    // IImGui��(��) ���� ��ӵ�
    void ImGuiRender() override;

    FolderTool() = default;
};

