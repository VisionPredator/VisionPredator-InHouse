#pragma once
#include "IImGui.h"
class SceneManager;
class Toolbar :
    public IImGui
{
public: 
    Toolbar();


    Toolbar(SceneManager* sceneManager);
    ~Toolbar()=default;

    void ImGuiRender() override;
    void Menu();
    void Help();
    void Serialize();
    void Dialog_SaveAs();
    bool m_IsPlaying=false;
    bool m_IsPause = false;
    std::string m_CurrentScenePath;
    SceneManager* m_SceneManager;
};

