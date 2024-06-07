#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
class Hierarchy :
    public IImGui
{
public:
    Hierarchy(SceneManager* scManager, HierarchySystem* hierarchySystem);

    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Hierarchy() = default;
private:
    HierarchySystem* m_HierarchySystem; 
    SceneManager* m_SceneManager;
};

