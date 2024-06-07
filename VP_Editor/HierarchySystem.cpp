#include "pch.h"
#include "HierarchySystem.h"
#include "Components.h"
#include <imgui.h>

HierarchySystem::HierarchySystem(SceneManager* sceneManager) :System(sceneManager)
{
}

void HierarchySystem::ShowEntitys()
{
	for (TransformComponent& transcomp :COMPITER(TransformComponent))
	{

		// 부모가 없으면 루트
		if (transcomp.GetComponent<Parent>())
			continue;

		ShowParentEntity(transcomp.GetEntityID());
	}
}

void HierarchySystem::ShowParentEntity(uint32_t entityID)
{
	bool isNodeOpened = false;
	ImGui::PushID(entityID);
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
	if (!m_SceneManager->HasComponent<Children>(entityID))
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (m_SelectedEntityID == entityID)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	auto IDcomp = m_SceneManager->GetComponent<IDComponent>(entityID);
	isNodeOpened = ImGui::TreeNodeEx(IDcomp->Name.c_str(), node_flags);


	if (isNodeOpened)
	{
		ImGui::TreePop();
	}

	bool test = ImGui::IsItemHovered();
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

 


	if (ImGui::IsItemClicked())
	{
		m_SelectedEntityID = entityID;
	}


	/// 아이템 드래그 앤 드랍
	ImGuiDragDropFlags target_flags = 0;
	// 드래그 소스가 되는 게임오브젝트
	if (ImGui::BeginDragDropSource(target_flags))
	{
		//int data = HierarchySystem::S
		ImGui::SetDragDropPayload("GameObject", &entityID, sizeof(uint32_t));

		ImGui::Text("%s", IDcomp->Name.data());
		ImGui::EndDragDropSource();
	}
	// 드랍이 될 게임오브젝트
	if (ImGui::BeginDragDropTarget())
	{
		ImGuiDragDropFlags target_flags = 0;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject", target_flags))
		{
			auto move_from = *(const int*)payload->Data;
			auto move_to = entityID;

			if (move_from != move_to)
			{
				//ParentChildrenSystem(m_GameObjectManager).AddChild(move_to, move_from);
			}

		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();



}
