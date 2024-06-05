#pragma once
#include "IImGui.h"
class Toolbar :
    public IImGui
{
public: 
    Toolbar();
    ~Toolbar()=default;

    void ImGuiRender() override;
    void Menu();
    void Help();
    void Serialize();
    bool m_IsPlaying=false;
    bool m_IsPause = false;
};

