#pragma once
#include "IImGui.h"
class SceneManager;
class HierarchySystem;
struct Component;
class Inspector:
    public IImGui
{
public:
    Inspector(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<HierarchySystem> hierarchySystem);
    // IImGui을(를) 통해 상속됨
    void ImGuiRender() override;

    Inspector() = default;
    void EntityImGui(uint32_t entitiyID);
    void TransformComponentImGui(Component* component);
    void ComponentImGui(Component* component);
    std::string RemoveComponentSuffix(const std::string& className);


	void MemberImGui(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector2(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector3(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Vector4(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Quaternion(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_Color(entt::meta_data memberMetaData, Component* component);

    void TypeImGui_string(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_wstring(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_bool(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_int(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_unsigned_int(entt::meta_data metadata, Component* component);
    void TypeImGui_double(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_float(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_EnumClass(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_vector_string(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_vector_wstring(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_list_uint32(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_vector_pair_wstring_float(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_vector_tuple_wstring_float_float(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_ColliderInfo(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_BoxColliderInfo(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_CapsuleColliderInfo(entt::meta_data memberMetaData, Component* component);
    void TypeImGui_SphereColliderInfo(entt::meta_data memberMetaData, Component* component);

	void TypeImGui_ControllerInfo(entt::meta_data memberMetaData, Component* component);
	void TypeImGui_CapsuleControllerInfo(entt::meta_data memberMetaData, Component* component);

    bool ShouldSkipComponent(entt::id_type metaTypeID);

	std::weak_ptr<SceneManager> m_SceneManager;
	std::weak_ptr<HierarchySystem> m_HierarchySystem;

	std::string m_searchComponent{};
	bool IsClicked = false;
	entt::id_type m_ClickedCompID{};
	float m_TypeBoxsize = 200.f; // 예를 들어 200 픽셀 너비
    friend class Entity;
};

