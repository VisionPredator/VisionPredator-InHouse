#include "pch.h"
#include "Hierarchy.h"
#include "SceneManager.h"
#include "HierarchySystem.h"
#include <EventManager.h>
#include <InputManager.h>

Hierarchy::Hierarchy(std::shared_ptr<SceneManager> scManager, std::shared_ptr<HierarchySystem> hierarchySystem) : m_SceneManager{ scManager }, m_HierarchySystem{ hierarchySystem }
{
}

void Hierarchy::ImGuiRender()
{
	ImGui::Begin("Hierarchy");
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	auto curcontext = ImGui::GetCurrentContext();
	auto currwindow = curcontext->CurrentWindow;
	ImVec2 LeftUp = { currwindow->Pos.x, currwindow->Pos.y };
	ImVec2 RightDown = { LeftUp.x + currwindow->Size.x,LeftUp.y + currwindow->Size.y };
	ImVec2 ClickedPos = { io.MouseClickedPos[0].x,io.MouseClickedPos[0].y };

	bool isOnHierarchy = ClickedPos.x > LeftUp.x
		&& ClickedPos.y > LeftUp.y
		&& ClickedPos.x < RightDown.x
		&& ClickedPos.y < RightDown.y;
	if (isOnHierarchy	&&ImGui::IsMouseClicked(0)
		)
	{
		m_HierarchySystem.lock()->m_SelectedEntityID = 0;
	}

	if (const ImGuiPayload* payload = ImGui::GetDragDropPayload(); payload && ImGui::IsMouseReleased(0) && isOnHierarchy)
	{
		auto move_from = *(const int*)payload->Data;
		if (m_SceneManager.lock()->HasEntity(move_from))
		{

		m_SceneManager.lock()->RemoveParent(move_from);
		}
	}


	if (!m_HierarchySystem.lock()->m_IsEntityRClicked &&ImGui::BeginPopupContextWindow())
	{
		if (ImGui::MenuItem("Create Empty"))
		{
			m_SceneManager.lock()->CreateEntity();
		}

		ImGui::EndPopup();
	}
	m_HierarchySystem.lock()->ShowEntitys();

	ImGui::End();

}
