#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
class Hierarchy :
    public IImGui
{
public:
    Hierarchy(std::shared_ptr<SceneManager> scManager, std::shared_ptr<HierarchySystem> hierarchySystem);

    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Hierarchy() = default;
private:

    std::weak_ptr<HierarchySystem> m_HierarchySystem;
    std::weak_ptr<SceneManager> m_SceneManager;
};

