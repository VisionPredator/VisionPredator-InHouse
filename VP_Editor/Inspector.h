#pragma once
#include "IImGui.h"
class Inspector :
    public IImGui
{
public:
    // IImGui��(��) ���� ��ӵ�
    void ImGuiRender() override;

    Inspector() = default;
};

