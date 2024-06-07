#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
class Inspector:
    public IImGui
{
public:
    Inspector(SceneManager* sceneManager, HierarchySystem* hierarchySystem);
    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Inspector() = default;
    SceneManager* m_SceneManager;
    HierarchySystem* m_HierarchySystem;
};

