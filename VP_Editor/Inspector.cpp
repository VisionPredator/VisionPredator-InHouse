#include "pch.h"
#include "Inspector.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
Inspector::Inspector(SceneManager* sceneManager, HierarchySystem* hierarchySystem) :m_SceneManager{ sceneManager }, m_HierarchySystem{ hierarchySystem }
{
}

void Inspector::ImGuiRender()
{
	ImGui::Begin("Inspector");
	const int entityID = m_HierarchySystem->m_SelectedEntityID;
	if (entityID != 0 && m_SceneManager->HasEntity(entityID))
	{

		ImGuiIO& io = ImGui::GetIO(); (void)io;



		ImGui::PushID(static_cast<int>(entityID));











	}

	ImGui::End();
}
