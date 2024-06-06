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


	if (ImGui::IsItemClicked(1))
	{
		ImGui::OpenPopup("Entity_Popup");
		m_RClickedEntityID = entityID;
		m_IsEntityRClicked = true;
	}
	if (ImGui::IsItemClicked())
	{
		m_SelectedEntityID = entityID;
	}


	if (m_IsEntityRClicked && ImGui::BeginPopupContextWindow("Entity_Popup"))
	{
		if (ImGui::MenuItem("Delete"))
		{
			//m_GameObjectManager->ReleaseGameObject(transform.GameObjectId);
			m_SceneManager->DeleteEntity(m_RClickedEntityID);
			m_RClickedEntityID = 0;
			m_IsEntityRClicked = false;
		}


		ImGui::EndPopup();
	}



	ImGui::PopID();
}
