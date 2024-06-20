#include "pch.h"
#include "FolderTool.h"
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <filesystem>
#include "SceneManager.h"
#include <iostream>
#include "Toolbar.h"
FolderTool::FolderTool(SceneManager* sceneManager) :m_SceneManager{ sceneManager }
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
		ImGui::BeginChild("ChildR", ImVec2(0, 0), ImGuiChildFlags_Border, window_flags);
		ImGui::InputText("Asset Search", &m_SearchName);
		ImGui::Separator();
		ImVec2 availableSize = ImGui::GetContentRegionAvail();
		ImVec2 tableSize = ImVec2(0, 0); // Default size
		FileImGui();

		ImGui::EndChild();
	}
	ImGui::End();

	// Open scene confirmation popup
	OpenCautionImGui();
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
	entries.reserve(32); // Reserve space to avoid reallocations
	for (const auto& entry : std::filesystem::directory_iterator(fPath))
	{
		entries.push_back(entry);
		if (entry.is_directory())
			hasSubDirectory = true;
	}

	ImGui::PushID(folderPath.c_str());
	ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
	if (!hasSubDirectory)
		node_flags |= ImGuiTreeNodeFlags_Leaf;
	if (m_CurrentFolderPath == folderPath)
		node_flags |= ImGuiTreeNodeFlags_Selected;
	const std::string folderName = fPath.filename().string();
	bool nodeOpen = ImGui::TreeNodeEx(folderName.c_str(), node_flags);

	if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
		m_CurrentFolderPath = folderPath;


	if (nodeOpen)
	{
		for (const auto& entry : entries)
			if (entry.is_directory())
				FolderImGui(entry.path().string());
		ImGui::TreePop();
	}
	ImGui::PopID();
}


void FolderTool::FileImGui()
{
	float availableSize = ImGui::GetContentRegionAvail().x;
	if (m_CurrentFolderPath.empty())
		return;

	ImVec2 buttonSize = ImVec2(80.f, 80.f);
	float currentImGuiX = 0;
	std::filesystem::path currentPath(m_CurrentFolderPath);
	if (!std::filesystem::exists(currentPath) || !std::filesystem::is_directory(currentPath))
		return;

	// Add "Up" button to navigate to the parent directory
	if (currentPath.has_parent_path() && m_AssetFolderPath != currentPath)
	{
		ImGui::PushID("..");
		currentImGuiX += buttonSize.x;
		if (ImGui::Button("..", buttonSize))
		{
			m_CurrentFolderPath = currentPath.parent_path().string();
		}
		ImGui::PopID();
		if (currentImGuiX < availableSize - 80)
			ImGui::SameLine();
		else
			currentImGuiX = 0;
	}

	for (const auto& entry : std::filesystem::directory_iterator(currentPath))
	{
		const auto& entrypath = entry.path();
		const std::string entryPathString = entrypath.string();
		const std::string filenameString = entrypath.filename().string();

		if (entry.is_directory())
		{
			
			ImGui::PushID(filenameString.c_str());
			if (currentImGuiX += buttonSize.x; ImGui::Button(filenameString.c_str(), buttonSize))
				m_CurrentFolderPath = entryPathString;
			ImGui::PopID();
		}
		else
		{
			std::string buttonLabel = filenameString;
			std::string tempLabel = buttonLabel;
			size_t pos = tempLabel.find('.');
			while (pos != std::string::npos)
			{
				tempLabel.insert(pos, "\n");
				pos = tempLabel.find('.', pos + 2); // Skip the inserted newline
			}

			ImGui::PushID(tempLabel.c_str());
			if (currentImGuiX += buttonSize.x; ImGui::Button(tempLabel.c_str(), buttonSize))
			{	// Handle button click
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
			{	// Handle double click
				if (!Toolbar::m_IsPlaying)
					if (entry.path().extension() == ".prefab")
					{
						// Handle double click for .prefab files
					}
					else if (entry.path().extension() == ".Scene")
					{
						// Handle double click for .Scene files
						m_ShowOpenSceneCaution = true;
						m_OpenScenePath = entryPathString;
					}
			}

			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(1) && !Toolbar::m_IsPlaying)
			{
				ImGui::OpenPopup("ItemOptions");
				ImGui::SetClipboardText(buttonLabel.c_str());
			}
			if (ImGui::BeginPopup("ItemOptions"))
			{
				if (entry.path().extension() == ".prefab" || entry.path().extension() == ".Scene" || entry.path().extension() == ".fbx")
				{
					if (ImGui::MenuItem("Copy Filename"))
					{
						std::filesystem::path relativePath = std::filesystem::relative(entrypath, m_AssetFolderPath);
						std::string relativeToString = relativePath.string();
						std::replace(relativeToString.begin(), relativeToString.end(), '\\', '/');
						std::string parentFolder = "../Data/";
						std::string copyFilePath = parentFolder + relativeToString;

						ImGui::SetClipboardText(copyFilePath.c_str());
					}
					if (ImGui::MenuItem("Delete"))
					{
						m_ShowDeleteCaution = true;
						m_DeleteFilePath = entry.path();
					}
				}
				ImGui::EndPopup();
			}
			//(tempLabel.c_str());
			ImGui::PopID();
		}

		if (currentImGuiX < availableSize-80)
			ImGui::SameLine();
		else
			currentImGuiX = 0;
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

void FolderTool::OpenCautionImGui()
{
	if (m_ShowOpenSceneCaution)
	{
		ImGui::OpenPopup("Scene_Change_Confirmation");
		m_ShowOpenSceneCaution = false;
	}

	if (ImGui::BeginPopupModal("Scene_Change_Confirmation", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Are you sure you want to open this scene?");
		ImGui::Separator();

		if (ImGui::Button("Yes", ImVec2(120, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnOpenScene", m_OpenScenePath);
			Toolbar::m_CurrentScenePath = m_OpenScenePath;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();
		if (ImGui::Button("No", ImVec2(120, 0)))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
}