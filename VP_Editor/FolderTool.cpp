#include "pch.h"
#include "FolderTool.h"
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include "SceneManager.h"
#include <iostream>
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
		//float a = ImGui::GetContentRegionAvail().x * 0.2f;
		ImGui::BeginChild("ChildL", ImVec2(300, 0), ImGuiChildFlags_None, window_flags);
		FolderImGui(m_AssetFolderPath);
		ImGui::EndChild();
		ImGui::SameLine();

	}
	// Right : 디렉토리 내부의 파일들을 버튼이나 뭐로 브라우징
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;
		// window_flags |= ImGuiWindowFlags_MenuBar;
		//ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
		ImGui::BeginChild("ChildR", ImVec2(0, 0), ImGuiChildFlags_Border, window_flags);
		ImGui::InputText("Asset Search", &m_SearchName);
		ImGui::Separator();
		FileImGui();

		ImGui::EndChild();
	}
	ImGui::End();

	// Delete confirmation popup
	DeleteCautionImGui();
}

void FolderTool::FolderImGui(const std::string& folderPath)
{
	std::filesystem::path fPath(folderPath);
	if (!std::filesystem::exists(fPath) || !std::filesystem::is_directory(fPath))
		return;

	bool hasSubDirectory = false;

	std::vector<std::filesystem::directory_entry> entries;
	for (const auto& entry : std::filesystem::directory_iterator(fPath))
	{
		entries.push_back(entry);
		if (entry.is_directory())
		{
			hasSubDirectory = true;
		}
	}

	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
	if (!hasSubDirectory)
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (m_CurrentFolderPath == folderPath)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	const std::string folderName = fPath.filename().string();

	if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0))
		m_CurrentFolderPath = folderPath;

	bool nodeOpen = ImGui::TreeNodeEx(folderName.c_str(), node_flags);
	if (nodeOpen)
	{
		for (const auto& entry : entries)
			if (entry.is_directory())
				FolderImGui(entry.path().string());
		ImGui::TreePop();
	}
}


void FolderTool::FileImGui()
{
	if (m_CurrentFolderPath.empty())
		return;

	ImVec2 buttonSize = ImVec2(80.f, 80.f);
	std::filesystem::path currentPath(m_CurrentFolderPath);
	if (!std::filesystem::exists(currentPath))
		return;
	if (!std::filesystem::is_directory(currentPath))
		return;

	// Add "Up" button to navigate to the parent directory
	if (currentPath.has_parent_path() && m_AssetFolderPath != currentPath)
	{
		ImGui::PushID("..");
		if (ImGui::Button("..", buttonSize))
		{
			m_CurrentFolderPath = currentPath.parent_path().string();
		}
		ImGui::PopID();
		ImGui::SameLine();
	}

	for (const auto& entry : std::filesystem::directory_iterator(currentPath))
	{
		ImGui::PushID(entry.path().string().c_str());
		if (entry.is_directory())
		{
			if (ImGui::Button(entry.path().filename().string().c_str(), buttonSize))
				m_CurrentFolderPath = entry.path().string();
		}
		else
		{
			std::string buttonLabel = entry.path().filename().string();
			std::string tempLabel = buttonLabel;
			size_t pos = tempLabel.find('.');
			while (pos != std::string::npos)
			{
				tempLabel.insert(pos, "\n");
				pos = tempLabel.find('.', pos + 2); // Skip the inserted newline
			}


			ImGui::PushID(tempLabel.c_str());

			if (ImGui::Button(tempLabel.c_str(), buttonSize))
			{
				if (entry.path().extension()== ".prefab")
				{

				}
				else if (entry.path().extension() == ".Scene")
				{

				}

			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_Right))
			{
				ImGui::OpenPopup("ItemOptions");
				ImGui::SetClipboardText(buttonLabel.c_str());
			}
			if (ImGui::BeginPopup("ItemOptions"))
			{
				if (ImGui::MenuItem("Copy Filename"))
				{
					ImGui::SetClipboardText(buttonLabel.c_str());
				}
				if (ImGui::MenuItem("Delete"))
				{
					// Handle delete action
					m_ShowDeleteCaution = true;
					m_DeleteFilePath = entry.path();

					//try
					//{

					//	if (std::filesystem::remove(entry.path()))
					//	{
					//	
					//		std::cout << "File deleted successfully: " << entry.path() << std::endl;
					//	}
					//	else
					//	{
					//		std::cerr << "File deletion failed: " << entry.path() << std::endl;
					//		path_to_delete = entry.path();
					//	
					//	}
					//}
					//catch (const std::filesystem::filesystem_error& e)
					//{
					//	std::cerr << "Filesystem error: " << e.what() << std::endl;
					//}
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		ImGui::SameLine();
		ImGui::PopID();
	}
}

void FolderTool::DeleteCautionImGui()
{
	if (m_ShowDeleteCaution)
	{
		ImGui::OpenPopup("Delete Confirmation");
		m_ShowDeleteCaution = false;
	}

	if (ImGui::BeginPopupModal("Delete Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to delete this file?");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			// 파일 삭제 로직
			try
			{
				if (std::filesystem::remove(m_DeleteFilePath))
					std::cout << "File deleted successfully: " << m_DeleteFilePath << std::endl;
				else
					std::cerr << "File deletion failed: " << m_DeleteFilePath << std::endl;
			}
			catch (const std::filesystem::filesystem_error& e)
			{
				std::cerr << "Filesystem error: " << e.what() << std::endl;
			}
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}
