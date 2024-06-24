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
    static bool m_IsPlaying;
    bool m_IsPause = false;
   static std::string m_CurrentScenePath;

    SceneManager* m_SceneManager;
};

