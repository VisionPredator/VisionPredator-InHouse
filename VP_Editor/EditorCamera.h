#pragma once
class EditorCamera
{
public: 
	EditorCamera();
	~EditorCamera()=default;
	void Initialize(double ratio);
	void Update(float deltatime);
	void CameraMove(float deltatime);
	void CameraRotation(float deltatime);
	void CalculateCamera();
	VPMath::Matrix World() const { return m_world; }
	VPMath::Matrix View() const { return m_view; }
	VPMath::Matrix Proj() const { return m_proj; }
	

private:
	float m_nearZ=0;
	float m_farZ=0;

	float m_FOV=0;
	float m_DeflaultSpeed = 10;
	float m_PowerSpeed = 20;

	float m_nearWindowHeight=0;
	float m_farWindowHeight=0;
	float m_ratio=1;
	bool m_PressedShift=false;
	VPMath::Vector3 m_Transform{};
	VPMath::Quaternion m_Quaternion=VPMath::Quaternion::Identity;
	VPMath::Vector3 m_FrontVector = {};
	VPMath::Vector3 m_UpVector = {};
	VPMath::Vector3 m_RightVector = {};
	VPMath::Matrix m_world;
	VPMath::Matrix m_view;
	VPMath::Matrix m_proj;
	VPMath::Matrix m_worldviewproj;
};

