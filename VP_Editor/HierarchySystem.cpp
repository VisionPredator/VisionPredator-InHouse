#include "pch.h"
#include "HierarchySystem.h"
#include "Components.h"
#include <imgui.h>
uint32_t HierarchySystem::m_SelectedEntityID=0;
HierarchySystem::HierarchySystem(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
}

void HierarchySystem::ShowEntitys()
{
	std::string lowerSearchingName = ToLower(m_SearchingName);
		ShowDelete = false;

	for (TransformComponent& transcomp : COMPITER(TransformComponent))
	{
		//std::string entityName = transcomp.GetComponent<IDComponent>()->Name;
		//std::string lowerEntityName = ToLower(entityName);
		if (transcomp.HasComponent<Parent>())
			continue;

		ShowParentEntity(transcomp.GetEntityID());
		//// Check if the entity name contains the search string (case-insensitive)
		//if (lowerSearchingName.empty() || lowerEntityName.find(lowerSearchingName) != std::string::npos)
		//{
		//	// Skip if the entity has a parent (not a root entity)

		//}
	}

}

void HierarchySystem::ShowParentEntity(uint32_t entityID)
{
	bool isNodeOpened = false;
	Children* entityChildren=nullptr;
	auto entity = GetSceneManager()->GetEntity(entityID);


	ImGui::PushID(entityID);
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow| ImGuiTreeNodeFlags_SpanFullWidth;

	if (!entity->HasComponent<Children>())
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	else
		entityChildren = entity->GetComponent<Children>();

	if (m_SelectedEntityID == entityID)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	auto IDcomp = entity->GetComponent<IDComponent>();



	isNodeOpened = ImGui::TreeNodeEx(IDcomp->Name.c_str(), node_flags);
	if (!ShowDelete)
	{
		if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::OpenPopup("Entity_Popup");
			m_RClickedEntityID = entityID;
			m_IsEntityRClicked = true;
			ShowDelete = true;
		}
		else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
		{
			ImGui::CloseCurrentPopup();
			m_RClickedEntityID = 0;
			m_IsEntityRClicked = false;
		}

	}





	if (m_IsEntityRClicked && ImGui::BeginPopupContextWindow("Entity_Popup"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			GetSceneManager()->DeleteEntity(m_RClickedEntityID);
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
				GetSceneManager()->AddChild(parentEntity, childEntity);
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::PopID();
	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
	{
		IsItemDoubleClicked = true;
	}

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
