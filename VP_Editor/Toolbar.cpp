#include "pch.h"
#include "Toolbar.h"
#include "ImGuiFileDialog.h"
#include "SceneManager.h"
#include "EventManager.h"
#include "../PhysxEngine/IPhysx.h"
bool Toolbar::m_IsPlaying = false;
Toolbar::Toolbar()
{
}
Toolbar::Toolbar(std::shared_ptr<SceneManager> sceneManager, Physic::IPhysx* physicinterface) :m_SceneManager{ sceneManager }, m_PhysicEngine{ physicinterface }
{
}
std::string Toolbar::m_CurrentScenePath = {};


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
		Help();
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

	ImGui::Text("Scene : %s", m_SceneManager.lock()->GetSceneName().c_str());


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
			m_SceneManager.lock()->SetSceneName(currentFileNameWithoutExt);

			std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			filePath += "/";

			filePath += m_SceneManager.lock()->GetSceneName();
			filePath += ".scene";
			m_SceneManager.lock()->SceneSerialize(filePath);
			m_CurrentScenePath = filePath;
		}
			// close
			ImGuiFileDialog::Instance()->Close();
	}
	// PhysicEngineImGui 함수 호출
	PhysicEngineImGui();
}

void Toolbar::Menu()
{
	if (ImGui::MenuItem("New Scene"))
	{
		m_CurrentScenePath.clear();
		m_SceneManager.lock()->OpenNewScene();
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
			m_SceneManager.lock()->SceneSerialize(m_CurrentScenePath);
		}
	}
	if (ImGui::MenuItem("Save as"))
	{
		Dialog_SaveAs();
	}
}

void Toolbar::Help()
{
	if (ImGui::MenuItem("PhysicSetting"))
	{
		m_ShowPhysicSettings = true;
	}
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

void Toolbar::PhysicEngineImGui()
{
	if (!m_ShowPhysicSettings) // Show the settings window if the flag is true
	{
		m_phsicsinfo = m_SceneManager.lock()->GetScenePhysic();
		return;
	}
	// Static cache for enum members to avoid recomputation
	auto enumMap = Reflection::GetEnumMap<EPhysicsLayer>();
	ImGui::PushID("PhysicsInfo");
	// Get the display size and calculate the centered position
	ImVec2 displaySize = ImGui::GetIO().DisplaySize;
	ImVec2 windowSize = ImVec2(800, 750);
	ImVec2 windowPos = ImVec2((displaySize.x - windowSize.x) / 2, ((displaySize.y - windowSize.y) / 2 -100));
	// Set the window size and position
	ImGui::SetNextWindowSize(windowSize, ImGuiCond_Always);
	ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
	if (ImGui::Begin("Physics Engine Settings"))
	{
		// 중력 벡터 편집
		ImGui::Text("Gravity");
		ImGui::SetNextItemWidth(200.f);
		ImGui::DragFloat3("##Gravity",&m_phsicsinfo.Gravity.x);
		//ImGui::InputFloat("Gravity X", &m_phsicsinfo.Gravity.x);
		//ImGui::InputFloat("Gravity Y", &m_phsicsinfo.Gravity.y);
		//ImGui::InputFloat("Gravity Z", &m_phsicsinfo.Gravity.z);

		// 프레임 레이트 편집
		ImGui::SetNextItemWidth(200.f);

		int tempFrameRate = static_cast<int>(m_phsicsinfo.FrameRate); // 임시 int 변수에 저장

		if (ImGui::InputInt("Frame Rate", &tempFrameRate)) 
		{
			if (tempFrameRate < 1) {
				tempFrameRate =1; // 음수 값을 0으로 설정
			}
			m_phsicsinfo.FrameRate = static_cast<uint32_t>(tempFrameRate); // uint32_t로 변환하여 저장
		}


		// 충돌 매트릭스 편집
		ImGui::Text("Collision Matrix");

		if (ImGui::BeginTable("CollisionMatrixTable", static_cast<int>(EPhysicsLayer::END) + 1, ImGuiTableFlags_Borders | ImGuiTableFlags_NoHostExtendX))
		{
			// Setup columns with fixed widths
			ImGui::TableSetupColumn(" ", ImGuiTableColumnFlags_WidthFixed, 100.0f); // First column for row labels
			for (int j = 0; j < static_cast<int>(EPhysicsLayer::END); ++j)
			{
				ImGui::TableSetupColumn(Reflection::GetName(enumMap[j]).c_str(), ImGuiTableColumnFlags_WidthFixed, 30.0f); // Each column with fixed width 30
			}

			// 첫 번째 행에 각 레이어 이름을 세로로 출력
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(" ");
			for (int j = 0; j < static_cast<int>(EPhysicsLayer::END); ++j)
			{
				ImGui::TableNextColumn();
				std::string layerName = Reflection::GetName(enumMap[j]);
				for (char c : layerName)
				{
					ImGui::Text("%c", c);
				}
			}

			// 각 행을 출력
			for (int i = 0; i < static_cast<int>(EPhysicsLayer::END); ++i)
			{
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("%s", Reflection::GetName(enumMap[i]).c_str()); // 행 이름 출력

				for (int j = 0; j < static_cast<int>(EPhysicsLayer::END); ++j)
				{
					ImGui::TableNextColumn();
					bool is_colliding = (m_phsicsinfo.CollisionMatrix[i] & (1 << j)) != 0;
					if (ImGui::Checkbox(("##" + std::to_string(i) + "_" + std::to_string(j)).c_str(), &is_colliding))
					{
						if (is_colliding)
						{
							m_phsicsinfo.CollisionMatrix[i] |= (1 << j);
							m_phsicsinfo.CollisionMatrix[j] |= (1 << i); // Also set the symmetric element
						}
						else
						{
							m_phsicsinfo.CollisionMatrix[i] &= ~(1 << j);
							m_phsicsinfo.CollisionMatrix[j] &= ~(1 << i); // Also clear the symmetric element
						}
					}
				}
			}
			ImGui::EndTable();
		}

		// Apply 버튼을 추가하여 변경 사항을 물리 엔진에 적용
		if (ImGui::Button("Apply"))
		{
			// 수정된 정보를 물리 엔진에 설정합니다.
			m_SceneManager.lock()->SetScenePhysic(m_phsicsinfo);
			m_PhysicEngine->SetPhysicsInfo(m_SceneManager.lock()->GetScenePhysic());
			EventManager::GetInstance().ImmediateEvent("OnSetPhysicUpdateRate", m_phsicsinfo.FrameRate);
			m_ShowPhysicSettings = false;
		}
	}
	ImGui::End();
	ImGui::PopID();
}