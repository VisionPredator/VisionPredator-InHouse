#include "pch.h"
#include "Inspector.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
#include "imgui_stdlib.h"

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

		auto& objectName = m_SceneManager->GetComponent<IDComponent>(entityID)->Name;
		ImGui::InputText("Name", &objectName);
		ImGui::Separator();
		ImGui::Text("Serialize");
		ImGui::SameLine();

		if (ImGui::Button("Save"))
		{
			int a = 5;
			a = 7;
		}

		ImGui::Separator();
		
		auto Components = m_SceneManager->GetOwnedComponent(entityID);
		for (Component* Comp: Components)
		{
			entt::id_type compID = Comp->GetTypeID();

			entt::meta_type  compMetaData = entt::resolve(compID);
			auto Component_MetaHandle =Comp->GetHandle();

			std::string componentName = Reflection::GetClass_Name(compMetaData);
			ImGui::Text(componentName.c_str());

			for (auto [MemberID, MemberData] : compMetaData.data())
			{
				MemberData.prop();

				auto MemberName = Reflection::GetName(MemberData);
				int testaste = 34;
				ImGui::Text(MemberName.c_str());

				testaste = 5;
			}

		}


	ImGui::PopID();
	}
	ImGui::End();
}
