#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
class Hierarchy :
    public IImGui
{
public:
    Hierarchy(SceneManager* scManager, HierarchySystem* hierarchySystem);

    // IImGui��(��) ���� ��ӵ�
    void ImGuiRender() override;

    Hierarchy() = default;
private:
    HierarchySystem* m_HierarchySystem; 
    SceneManager* m_SceneManager;
};

