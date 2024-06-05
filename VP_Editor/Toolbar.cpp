#include "pch.h"
#include "Toolbar.h"
#include <EventManager.h>

Toolbar::Toolbar()
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
			m_IsPlaying = true;
		}

	}
	else
	{
		if (ImGui::Button("Stop", ImVec2(60, 0)))
		{
			EventManager::GetInstance().ScheduleEvent("OnStopButton");
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
	ImGui::EndMainMenuBar();
}

void Toolbar::Menu()
{
	if (ImGui::MenuItem("New Scene"))
	{
	}
	if (ImGui::MenuItem("Open"))
	{
	}



}

void Toolbar::Help()
{
}

void Toolbar::Serialize()
{
}
