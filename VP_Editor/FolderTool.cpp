#include "pch.h"
#include "FolderTool.h"
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include "SceneManager.h"
FolderTool::FolderTool(SceneManager* sceneManager) :m_SceneManager{sceneManager}
{
	auto currPath = std::filesystem::current_path();

	auto ABPath = currPath.parent_path();
	ABPath.append("Data");
	m_AssetFolderPath = ABPath.string();
	m_CurrentFolderPath = m_AssetFolderPath;
}

void FolderTool::ImGuiRender()
{
	ImGui::Begin("Project");
	//Left : 디렉토리 표시
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
		ImGui::BeginChild("ChildL", ImVec2(ImGui::GetContentRegionAvail().x * 0.3f, 0), ImGuiChildFlags_None, window_flags);
		FolderImGui(m_AssetFolderPath);
		ImGui::EndChild();
		ImGui::SameLine();

	}
	// Right : 디렉토리 내부의 파일들을 버튼이나 뭐로 브라우징
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		// window_flags |= ImGuiWindowFlags_MenuBar;
		ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 0), ImGuiChildFlags_Border, window_flags);
		ImGui::InputText("Asset Search", &m_SearchName);
		ImGui::Separator();

		if (!m_CurrentFolderPath.empty())
		{
			ImVec2 buttonSize = ImVec2(100.0f, 100.0f);
			std::filesystem::path currentPath(m_CurrentFolderPath);
			if (std::filesystem::exists(currentPath) && std::filesystem::is_directory(currentPath))
			{
				// Customize button style for Unity-like feel
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.23f, 0.23f, 0.23f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.25f, 0.25f, 0.25f, 1.0f));
				ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 4));
				// Add "Up" button to navigate to the parent directory
				if (currentPath.has_parent_path() && m_AssetFolderPath != currentPath)
				{
					if (ImGui::Button("..", buttonSize))
					{
						m_CurrentFolderPath = currentPath.parent_path().string();
					}
					ImGui::SameLine();
				}

				for (const auto& entry : std::filesystem::directory_iterator(currentPath))
				{
					if (entry.is_directory())
					{
						if (ImGui::Button(entry.path().filename().string().c_str(), buttonSize))
						{
							m_CurrentFolderPath = entry.path().string();
						}
							ImGui::SameLine();
					}
					else
					{
						ImGui::Text("%s", entry.path().filename().string().c_str());
						ImGui::SameLine();
					}
				}
			}
			// Pop styles after use
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(3);
		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
	ImGui::End();
}

void FolderTool::FolderImGui(const std::string& folderPath)
{
	std::filesystem::path fPath(folderPath);
	if (!std::filesystem::exists(fPath) || !std::filesystem::is_directory(fPath))
		return;
	bool hasSubDirectory = false;

	for (const auto& entry : std::filesystem::directory_iterator(fPath))
	{
		if (entry.is_directory())
		{
			hasSubDirectory = true;
			break;
		}
	}

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow |ImGuiTreeNodeFlags_SpanFullWidth;
	if (!hasSubDirectory)
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (m_CurrentFolderPath == folderPath)
		node_flags |= ImGuiTreeNodeFlags_Selected;

	bool nodeOpen = ImGui::TreeNodeEx(fPath.filename().string().c_str(), node_flags);
	if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
	{
		m_CurrentFolderPath = folderPath;
	}


	if (nodeOpen) {
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.is_directory()) {
				FolderImGui(entry.path().string());
			}
		}
		ImGui::TreePop();
	}
}

void FolderTool::FileImGui()
{
}
