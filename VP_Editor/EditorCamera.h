#pragma once
#include "IImGui.h"
#include "../VPCommon/EventSubscriber.h"
class SceneManager;
class EditorCamera:public IImGui,public EventSubscriber
{
public: 
	EditorCamera(std::shared_ptr<SceneManager> sceneManager);
	~EditorCamera()=default;
	void Initialize();
	void Update(float deltatime);
	void CameraMove(float deltatime);
	void MoveableSetting();
	void CameraRotation();
	void CalculateCamera();
	void CalculateCameraTransform();
	void DoubleClicked(float deltatime);
	void OnResize(std::any hwnd);
	VPMath::Vector3 GetPose() { return m_Location; };
	VPMath::Vector3 GetUp() { return m_UpVector; };
	VPMath::Vector3 GetForward() { return m_ForwardVector; };
	// IImGui을(를) 통해 상속됨
	void ImGuiRender() override;
	VPMath::Matrix GetView() const { return m_view; }
	VPMath::Matrix GetProj() const { return m_proj; }
	VPMath::Matrix GetOrthoProj() const { return m_orthoProj; }
	bool IsMoveable{};

private:
	float m_sensitivity= 0.1f;


	float m_nearZ = 0.f;
	float m_farZ = 0.f;
	VPMath::Matrix m_view;
	VPMath::Matrix m_proj;
	VPMath::Matrix m_orthoProj;
	const float m_maxPitch = 89.f; // Convert degrees to radians

	float m_FOV = 0.f;
	float m_DeflaultSpeed = 10.f;
	float m_ShiftSpeed = 50.f;
	uint32_t m_Width = 1;
	uint32_t m_Height = 1;
	float m_ratio = 1.f;
	bool m_PressedShift = false;
	VPMath::Vector3 m_Location{};
	VPMath::Vector3 m_Rotation{};
	VPMath::Vector3 m_Scale{ 1.f,1.f,1.f };
	VPMath::Matrix m_Transform = VPMath::Matrix::Identity;
	VPMath::Quaternion m_Quaternion = VPMath::Quaternion::Identity;
	VPMath::Vector3 m_ForwardVector = { 0.f,0.f,1.f };
	VPMath::Vector3 m_UpVector = {};
	VPMath::Vector3 m_RightVector = {};
	VPMath::Vector3 m_LerpStartPos = {};
	VPMath::Vector3 m_LerpEndPos = {};
	float m_LerpEndTime = 0.2f;
	float m_LerpTime = 0.f;

public:
	std::weak_ptr<SceneManager> m_SceneManager;
};

