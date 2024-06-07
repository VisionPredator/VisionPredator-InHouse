#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
class Inspector:
    public IImGui
{
public:
    Inspector(SceneManager* sceneManager, HierarchySystem* hierarchySystem);
    // IImGui��(��) ���� ��ӵ�
    void ImGuiRender() override;

    Inspector() = default;
    SceneManager* m_SceneManager;
    HierarchySystem* m_HierarchySystem;
};

