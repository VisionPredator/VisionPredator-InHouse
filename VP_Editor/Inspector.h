#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
struct Component;
class Inspector:
    public IImGui
{
public:
    Inspector(SceneManager* sceneManager, HierarchySystem* hierarchySystem);
    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Inspector() = default;
    void ComponentImGui(Component* component);
	void MemberImGui(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector2(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector3(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector4(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Quaternion(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Color(entt::meta_data memberMetaData, Component* component);

    void TypeImGui_string(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_bool(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_int(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_unsigned_int(entt::meta_data metadata, Component* component);
    void TypeImGui_double(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_float(entt::meta_data memberMetaData, Component* component);

    SceneManager* m_SceneManager;
    HierarchySystem* m_HierarchySystem;
    std::string m_searchComponent{};
    
};

