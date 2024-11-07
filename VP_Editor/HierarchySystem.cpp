#include "pch.h"
#include "HierarchySystem.h"
#include "Components.h"
#include <imgui.h>
#include "EventManager.h"
uint32_t HierarchySystem::m_SelectedEntityID = 0;
HierarchySystem::HierarchySystem(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnPlayButton", CreateSubscriber(&HierarchySystem::OnPlayButton));
	EventManager::GetInstance().Subscribe("OnStopButton", CreateSubscriber(&HierarchySystem::OnStopButton));
}

void HierarchySystem::ShowEntitys()
{
	std::string lowerSearchingName = ToLower(m_SearchingName);
	ShowDelete = false;

	if (lowerSearchingName.empty())
	{
		auto Entitymap = GetSceneManager()->GetEntityMap();
		m_EntityCount = static_cast<uint32_t>(Entitymap.size());
		// Display the total number of entities
		ImGui::Text("Total Entities: %zu", m_EntityCount);
		COMPLOOP(TransformComponent, transcomp)
		{

			if (transcomp.HasComponent<Parent>())
				continue;

			ShowParentEntity(transcomp.GetEntityID());

		}
	}
	else
	{
		std::vector<uint32_t> showingEntitIDs;
		m_EntityCount = 0;
		COMPLOOP(IDComponent, idcomp)
		{
			std::string entityName = idcomp.Name;
			std::string lowerEntityName = ToLower(entityName);
			if (lowerSearchingName.empty() || lowerEntityName.find(lowerSearchingName) != std::string::npos)
			{
				showingEntitIDs.push_back(idcomp.GetEntityID());
				m_EntityCount++;
			}
		}
		ImGui::Text("Total Searched Entitys: %zu", m_EntityCount);
		for (const auto& entityid : showingEntitIDs)
			ShowEntity(entityid);

		showingEntitIDs.clear();
	}
}

void HierarchySystem::ShowEntity(uint32_t entityID)
{
	auto entity = GetSceneManager()->GetEntity(entityID);
	if (!entity) return;

	auto IDcomp = entity->GetComponent<IDComponent>();

	ImGui::PushID(entityID);

	// 엔티티 이름을 선택 가능한 형태로 표시
	if (ImGui::Selectable(IDcomp->Name.c_str(), m_SelectedEntityID == entityID)) {
		m_SelectedEntityID = entityID;
	}

	if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
	{
		IsItemDoubleClicked = true;
	}

	// 마우스 오른쪽 클릭을 통해 컨텍스트 메뉴 표시
	if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right)) {
		ImGui::OpenPopup("Entity_Popup");
		m_RClickedEntityID = entityID;
		m_IsEntityRClicked = true;
		ShowDelete = true;
	}

	if (m_IsEntityRClicked && ImGui::BeginPopup("Entity_Popup")) {
		if (ImGui::MenuItem("Delete")) {
			GetSceneManager()->DeleteEntity(m_RClickedEntityID);
			m_RClickedEntityID = 0;
			m_IsEntityRClicked = false;
		}
		ImGui::EndPopup();
	}

	// 드래그 앤 드롭 기능 추가
	if (!IsPlayMode)
	{
		ImGuiDragDropFlags target_flags = 0;

		// 드래그 소스 설정
		if (ImGui::BeginDragDropSource(target_flags))
		{
			ImGui::SetDragDropPayload("Entity", &entityID, sizeof(uint32_t));
			ImGui::Text("%s", IDcomp->Name.c_str());
			ImGui::EndDragDropSource();
		}

		// 드롭 타겟 설정
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Entity", target_flags))
			{
				auto childEntity = *(const uint32_t*)payload->Data;
				auto parentEntity = entityID;

				if (childEntity != parentEntity)
					GetSceneManager()->AddChild(parentEntity, childEntity);
			}
			ImGui::EndDragDropTarget();
		}
	}

	ImGui::PopID();
}


void HierarchySystem::OnPlayButton(std::any none)
{
	IsPlayMode = true;
}

void HierarchySystem::OnStopButton(std::any none)
{
	IsPlayMode = false;
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
	if (!IsPlayMode)
	{
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

