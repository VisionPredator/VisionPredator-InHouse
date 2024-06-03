#pragma once
#include "IImGui.h"
class Toolbar :
    public IImGui
{
public: 
    Toolbar();
    ~Toolbar()=default;

    void ImGuiRender() override;

};

