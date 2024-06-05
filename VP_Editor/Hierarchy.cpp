#include "pch.h"
#include "Hierarchy.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
#include <EventManager.h>

Hierarchy::Hierarchy(SceneManager* scManager): m_SceneManager{scManager}
{
	m_HierarchySystem = new HierarchySystem(scManager);
}

void Hierarchy::ImGuiRender()
{
	ImGui::Begin("Hierarchy");


	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty"))
		{
			m_SceneManager->CreateEntity();
		}

		ImGui::EndPopup();
	}
	m_HierarchySystem->ShowEntitys();
	ImGui::End();

}
