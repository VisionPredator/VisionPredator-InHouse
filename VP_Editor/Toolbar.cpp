#include "pch.h"
#include "Toolbar.h"
#include "ImGuiFileDialog.h"
#include "SceneManager.h"
#include <EventManager.h>
bool Toolbar::m_IsPlaying = false;
Toolbar::Toolbar()
{
}
std::string Toolbar::m_CurrentScenePath = {};
Toolbar::Toolbar(SceneManager* sceneManager): m_SceneManager { sceneManager }
{
}

void Toolbar::ImGuiRender()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Menu"))
	{
		Menu();
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		ImGui::EndMenu();
	}

	ImGui::SameLine(ImGui::GetWindowWidth() / 2 - 63);
	if (!m_IsPlaying)
	{
		if (ImGui::Button("Play", ImVec2(60, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnPlayButton");
			EventManager::GetInstance().ScheduleEvent("OnSaveCurrentToTemp");


			m_IsPlaying = true;
		}

	}
	else
	{
		if (ImGui::Button("Stop", ImVec2(60, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnStopButton");
			EventManager::GetInstance().ScheduleEvent("OnOverwriteTempToCurrent");
			m_IsPlaying = false;
		}
	}

	if (!m_IsPause)
	{
		if (ImGui::Button("Pause", ImVec2(60, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnPauseButton");
			m_IsPause = true;
		}

	}
	else
	{
		if (ImGui::Button("Resume", ImVec2(60, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnResumeButton");
			m_IsPause = false;

		}
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 300);

	ImGui::Text("Scene : %s", m_SceneManager->GetSceneName().c_str());


	ImGui::EndMainMenuBar();

	if (ImGuiFileDialog::Instance()->Display("ChooseSceneDlgKey", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, { 1000, 500 }))
	{
		if (ImGuiFileDialog::Instance()->IsOk()) 
		{ // action if OK
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			m_CurrentScenePath = filePathName;
			EventManager::GetInstance().ScheduleEvent("OnOpenScene", filePathName);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
	}


	if (ImGuiFileDialog::Instance()->Display("SaveAsDlgKey", ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove, { 1000, 500 }))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{ // action if OK
			std::string currentFileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			std::string currentFileNameWithoutExt;
			size_t lastindex = currentFileName.find_last_of(".");

			currentFileNameWithoutExt = currentFileName.substr(0, lastindex);
			m_SceneManager->SetSceneName(currentFileNameWithoutExt);

			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			filePath += "/";

			filePath += m_SceneManager->GetSceneName();
			filePath += ".scene";
			m_SceneManager->SceneSerialize(filePath);
			m_CurrentScenePath = filePath;
		}
			// close
			ImGuiFileDialog::Instance()->Close();
	}

}

void Toolbar::Menu()
{
	if (ImGui::MenuItem("New Scene"))
	{
		m_CurrentScenePath.clear();
		m_SceneManager->OpenNewScene();
	}
	if (ImGui::MenuItem("Open"))
	{
		IGFD::FileDialogConfig config;
		config.path = "../Data/Scene";
		config.flags = ImGuiFileDialogFlags_Modal;
		ImGuiFileDialog::Instance()->OpenDialog("ChooseSceneDlgKey", "Choose Scene", ".scene", config);
	}
	if (ImGui::MenuItem("Save"))
	{
		if (m_CurrentScenePath.empty())
			Dialog_SaveAs();
		else
		{
			m_SceneManager->SceneSerialize(m_CurrentScenePath);
		}
	}
	if (ImGui::MenuItem("Save as"))
	{
		Dialog_SaveAs();
	}
}

void Toolbar::Help()
{
}

void Toolbar::Serialize()
{
}

void Toolbar::Dialog_SaveAs()
{
	IGFD::FileDialogConfig config;
	config.path = "../Data/Scene";
	config.flags = ImGuiFileDialogFlags_Modal | ImGuiFileDialogFlags_ConfirmOverwrite;
	ImGuiFileDialog::Instance()->OpenDialog("SaveAsDlgKey", "Save As...", ".scene", config);

}