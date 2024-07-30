#pragma once
#include "IImGui.h"
#include "../PhysxEngine/VPPhysicsStructs.h"

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
	static bool m_IsPlaying;
	bool m_IsPause = false;
	static std::string m_CurrentScenePath;
	bool m_ShowPhysicSettings{ false };
	std::weak_ptr<SceneManager> m_SceneManager;
	Physic::IPhysx* m_PhysicEngine = nullptr;
	VPPhysics::PhysicsInfo m_phsicsinfo{};
};

