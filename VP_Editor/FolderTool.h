#pragma once
#include "IImGui.h"
#include "../include/imgui.h"
class SceneManager;
class FolderTool :
    public IImGui
{
public:
    // IImGui을(를) 통해 상속됨
    FolderTool(SceneManager* sceneManager);
    void ImGuiRender() override;

    FolderTool() = default;
    void FolderImGui(const std::string& folderPath);
    void FileImGui();
    std::string m_SearchName;
    std::string m_AssetFolderPath{};
    std::string m_CurrentFolderPath{};
    SceneManager* m_SceneManager;
};

