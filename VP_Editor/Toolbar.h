#pragma once
#include "IImGui.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
#include "NavStructs.h"

class SceneManager;
namespace Physic
{
    class IPhysx;
}
class Toolbar :
    public IImGui
{
public: 
    Toolbar();


	Toolbar(std::shared_ptr<SceneManager> sceneManager, Physic::IPhysx* physicinterface);
	~Toolbar() = default;

	void ImGuiRender() override;
	void Menu();
	void Help();
	void Serialize();
	void Dialog_SaveAs();
	void PhysicEngineImGui();
	void NavMeshImGui();
	static bool m_IsPlaying;
	static bool m_IsPaused;
	static std::string m_CurrentScenePath;
	bool m_ShowPhysicSettings{ false };
	bool m_ShowNavSettings{ false };
	std::weak_ptr<SceneManager> m_SceneManager;
	Physic::IPhysx* m_PhysicEngine = nullptr;
	VPPhysics::PhysicsInfo m_PhysicInfo_ImGui{};
	BuildSettings m_NavBuildSetteing_ImGui{};
};

