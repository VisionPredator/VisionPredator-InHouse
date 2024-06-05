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

void HierarchySystem::ShowParentEntity(uint32_t entityid)
{
	bool isNodeOpened = false;
	ImGui::PushID(entityid);
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanFullWidth;
	if (!m_SceneManager->HasComponent<Children>(entityid))
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (m_SelectedNumber == entityid)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	auto IDcomp = m_SceneManager->GetComponent<IDComponent>(entityid);
	isNodeOpened = ImGui::TreeNodeEx(IDcomp->Name.c_str(), node_flags);

	if (isNodeOpened)
	{
		ImGui::TreePop();
	}
	ImGui::PopID();
}
