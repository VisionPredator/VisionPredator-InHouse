#pragma once
#include "IImGui.h"
#include "EditorCamera.h"
#include "EventSubscriber.h"
class SceneManager;
using namespace VPMath;
namespace Graphics
{
	class Interface;
}
class EditorViewPort :public IImGui, public EventSubscriber
{

	enum class RENDERMODE
	{
		Albedo,
		Normal,
		Position,
		Depth,
		Metalic_Roughness, 
		LightMap,
		Emissive,
		GBuffer,
		END
	};
public:
	EditorViewPort(std::shared_ptr<SceneManager> sceneManager, std::shared_ptr<EditorCamera> Camera, Graphics::Interface* Graphics);
	// IImGui을(를) 통해 상속됨
	void ImGuiRender() override;
	void PlayingImGui();
	void EditingImGui();
	void EditViewPortImGui(std::wstring mode,ImVec2 pos, ImVec2 size);
	void ImGuizmoRender();
	void ImGuizmoSettingrender();
	void RenderImGuiViewport();
	void OnResize(std::any hwnd);
	std::wstring GetRenderModeString(RENDERMODE renderMode);


private:
	RENDERMODE m_CurrentRenderMode = RENDERMODE::GBuffer;
	Vector3 m_TranslationSnapValue = Vector3(1.0f);
	Vector3 m_RotationSnapValue = Vector3(15.0f);
	Vector3 m_ScaleSnapValue = Vector3(0.1f);
	Vector3* m_CurrentModeSnap = &m_TranslationSnapValue;

	std::weak_ptr<SceneManager> m_SceneManager;
	std::weak_ptr<EditorCamera> m_Camera;
	Graphics::Interface* m_Graphics;
	ImGuizmo::OPERATION m_ImGuizmoMode = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE m_GuizmoMode= ImGuizmo::LOCAL;
	bool m_IsLocalMode = true;
	bool m_IsSocket{};
	VPMath::Vector2 m_Rectsize;
	ImVec2 m_DrawPos{};
	ImVec2	m_DrawSize{};
};

