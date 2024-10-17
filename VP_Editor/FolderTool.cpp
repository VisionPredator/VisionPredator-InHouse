#include "pch.h"
#include "FolderTool.h"
#include <filesystem>
#include "SceneManager.h"
#include <iostream>
#include "Toolbar.h"
FolderTool::FolderTool(std::shared_ptr<SceneManager> sceneManager) :m_SceneManager{ sceneManager }
{
	auto currPath = std::filesystem::current_path();

	auto ABPath = currPath.parent_path();
	ABPath.append("Data");
	m_AssetFolderPath = ABPath.string();
	m_CurrentFolderPath = m_AssetFolderPath;
}
void OpenFileWithDefaultViewer(const std::string& filePath) 
{
#ifdef _WIN32
	std::string command = "start " + filePath;
	std::system(command.c_str());  // This will open the file in its default application
#endif
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

	ImVec2 buttonSize = ImVec2(100.f, 80.f);
	float currentImGuiX = 0;
	std::filesystem::path currentPath(m_CurrentFolderPath);
	if (!std::filesystem::exists(currentPath) || !std::filesystem::is_directory(currentPath))
		return;

	// Add "Up" button to navigate to the parent directory
	if (currentPath.has_parent_path() && m_AssetFolderPath != currentPath) {
		ImGui::PushID("..");
		currentImGuiX += buttonSize.x;
		if (ImGui::Button("..", buttonSize)) {
			m_CurrentFolderPath = currentPath.parent_path().string();
		}
		ImGui::PopID();
		if (currentImGuiX < availableSize - 130.f)
			ImGui::SameLine();
		else
			currentImGuiX = 0;
	}

	// Display folder and file items based on the search term
	for (const auto& entry : std::filesystem::directory_iterator(currentPath)) 
	{
		const auto& entrypath = entry.path();
		const std::string entryPathString = entrypath.string();
		const std::string filenameString = entrypath.filename().string();

		// Exclude ".SVN" folder
		if (filenameString == ".svn")
			continue;

		// Search functionality: only display items that match the search term
		if (!m_SearchName.empty()) {
			std::string lowerFilename = filenameString;
			std::string lowerSearch = m_SearchName;

			// Convert to lowercase for case-insensitive search
			std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
			std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

			// If the filename doesn't contain the search term, skip it
			if (lowerFilename.find(lowerSearch) == std::string::npos) {
				continue;
			}
		}

		// Calculate the text size based on current font and handle long names
		std::string displayName = filenameString;
		std::string tempLabel = displayName;

		// Insert newlines before the extension period to wrap long names
		size_t pos = tempLabel.find('.');
		if (pos != std::string::npos && pos != 0) {
			tempLabel.insert(pos, "\n");
		}

		if (entry.is_directory()) {
			ImGui::PushID(filenameString.c_str());
			if (currentImGuiX += buttonSize.x; ImGui::Button(tempLabel.c_str(), buttonSize))
				m_CurrentFolderPath = entryPathString;
			ImGui::PopID();
		}
		else {
			ImGui::PushID(tempLabel.c_str());
			if (currentImGuiX += buttonSize.x; ImGui::Button(tempLabel.c_str(), buttonSize)) {
				// Handle button click
			}
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				// Handle double click for files
				if (!Toolbar::m_IsPlaying) {
					if (entry.path().extension() == ".prefab") {
						m_SceneManager.lock()->DeSerializePrefab(entryPathString);
					}
					else if (entry.path().extension() == ".scene") {
						m_ShowOpenSceneCaution = true;
						m_OpenScenePath = entryPathString;
					}
					if (entry.path().extension() == ".png" || entry.path().extension() == ".dds" || entry.path().extension() == ".mp3" || entry.path().extension() == ".fbx") {
						OpenFileWithDefaultViewer(entryPathString);
					}
				}
			}

			// Handle right-click context menu
			if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(1) && !Toolbar::m_IsPlaying)
			{
				ImGui::OpenPopup("ItemOptions");
			}
			if (ImGui::BeginPopup("ItemOptions"))
			{
				if (ImGui::MenuItem("Open File")) { OpenFileWithDefaultViewer(entryPathString); }
				if (ImGui::MenuItem("Show in Explorer"))
				{
#ifdef _WIN32 					
					// Open the file's directory in explorer 				
					std::string command = "explorer /select," + entryPathString;
					system(command.c_str());
#endif 				
				}
				if (entry.path().extension() == ".prefab" || entry.path().extension() == ".scene")
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
				}
				else if (entry.path().extension() == ".fbx") {
					if (ImGui::MenuItem("Copy Filename"))
					{
						//Get the filename with extension 						
						std::string filenameWithExtension = entry.path().filename().string();
						ImGui::SetClipboardText(filenameWithExtension.c_str());
					}
				}
				else if (entry.path().extension() == ".mp3" || entry.path().extension() == ".wav")
				{
					if (ImGui::MenuItem("Copy Filename"))
					{ 						// Get the filename without extension 
						std::string filenameWithoutExtension = entry.path().stem().string();
						ImGui::SetClipboardText(filenameWithoutExtension.c_str());
					}
				}
				else if (entry.path().extension() == ".png" || entry.path().extension() == ".dds") {
					if (ImGui::MenuItem("Copy Filename"))
					{
						// Get the filename with extension 	
						std::string filenameWithExtension = entry.path().filename().string();
						ImGui::SetClipboardText(filenameWithExtension.c_str());
					}
				}
				if (ImGui::MenuItem("Delete"))
				{
					m_ShowDeleteCaution = true;
					m_DeleteFilePath = entry.path();
				}
				ImGui::EndPopup();
			} 			ImGui::PopID();
		}

		if (currentImGuiX < availableSize - 130.f)
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