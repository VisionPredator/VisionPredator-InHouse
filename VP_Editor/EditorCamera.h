#pragma once
#include "IImGui.h"
class SceneManager;
class EditorCamera:public IImGui
{
public: 
	EditorCamera(SceneManager* sceneManager);
	~EditorCamera()=default;
	void Initialize();
	void Update(float deltatime);
	void CameraMove(float deltatime);
	void CameraRotation();
	void CalculateCamera();
	void DoubleClicked(float deltatime);
	void CameraRotation2();
	void CalculateCamera2();

	// IImGui을(를) 통해 상속됨
	void ImGuiRender() override;
	VPMath::Matrix GetView() const { return m_view; }
	VPMath::Matrix GetProj() const { return m_proj; }
	

private:
	float m_sensitivity= 0.1f;


	float m_nearZ=0;
	float m_farZ=0;
	VPMath::Matrix m_view;
	VPMath::Matrix m_proj;
	const float m_maxPitch =89.f; // Convert degrees to radians

	float m_FOV=0;
	float m_DeflaultSpeed = 10;
	float m_ShiftSpeed = 50;
	uint32_t m_Width=1;
	uint32_t m_Height=1;
	float m_ratio=1;
	bool m_PressedShift=false;
	VPMath::Vector3 m_Transform{};
	VPMath::Vector3 m_Rotation{};
	VPMath::Quaternion m_Quaternion=VPMath::Quaternion::Identity;
	VPMath::Vector3 m_FrontVector = {};
	VPMath::Vector3 m_UpVector = {};
	VPMath::Vector3 m_RightVector = {};
	VPMath::Vector3 m_LerpStartPos = {};
	VPMath::Vector3 m_LerpEndPos = {};
	float m_LerpEndTime = 0.2;
	float m_LerpTime = 0;

public:
	SceneManager* m_SceneManager;
};

