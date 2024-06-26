#include "pch.h"
#include "HierarchySystem.h"
#include "Components.h"
#include <imgui.h>
uint32_t HierarchySystem::m_SelectedEntityID=0;
HierarchySystem::HierarchySystem(SceneManager* sceneManager) :System(sceneManager)
{
}

void HierarchySystem::ShowEntitys()
{


	for (TransformComponent& transcomp :COMPITER(TransformComponent))
	{

		// 부모가 없으면 루트
		if (transcomp.HasComponent<Parent>())
			continue;

		ShowParentEntity(transcomp.GetEntityID());
	}

}

void HierarchySystem::ShowParentEntity(uint32_t entityID)
{
	bool isNodeOpened = false;
	Children* entityChildren=nullptr;
	Entity* entity = m_SceneManager->GetEntity(entityID);


	ImGui::PushID(entityID);
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;

	if (!entity->HasComponent<Children>())
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	else
		entityChildren = entity->GetComponent<Children>();

	if (m_SelectedEntityID == entityID)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	auto IDcomp = entity->GetComponent<IDComponent>();
	isNodeOpened = ImGui::TreeNodeEx(IDcomp->Name.c_str(), node_flags);




	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("Entity_Popup");
		m_RClickedEntityID = entityID;
		m_IsEntityRClicked = true;
	}




	if (m_IsEntityRClicked && ImGui::BeginPopupContextWindow("Entity_Popup"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			m_SceneManager->DeleteEntity(m_RClickedEntityID);
			m_RClickedEntityID = 0;
			m_IsEntityRClicked = false;
		}
		ImGui::EndPopup();
	}


	/// 아이템 드래그 앤 드랍
	ImGuiDragDropFlags target_flags = 0;
	// 드래그 Entity
	if (ImGui::BeginDragDropSource(target_flags))
	{
		ImGui::SetDragDropPayload("Entity", &entityID, sizeof(uint32_t));

		ImGui::Text("%s", IDcomp->Name.data());
		ImGui::EndDragDropSource();
	}
	// 드랍이 Entity
	if (ImGui::BeginDragDropTarget())
	{
		ImGuiDragDropFlags target_flags = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity", target_flags))
		{
			auto childEntity = *(const int*)payload->Data;
			auto parentEntity = entityID;

			if (childEntity != parentEntity)
				m_SceneManager->AddChild(parentEntity, childEntity);
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();

	if (ImGui::IsItemClicked())
	{
		m_SelectedEntityID = entityID;
	}

	if (isNodeOpened)
	{
		if (entityChildren == nullptr)
		{
			ImGui::TreePop();
			return;
		}
		// 게임 오브젝트의 자식들에 대한 루프
		for (auto k : entityChildren->ChildrenID)
			ShowParentEntity(k);
		// 트리 노드 닫기
		ImGui::TreePop();
	}
}
