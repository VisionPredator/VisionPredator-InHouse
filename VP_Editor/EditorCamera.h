#pragma once
class EditorCamera
{
public: 
	EditorCamera();
	~EditorCamera()=default;
	void Initialize(double ratio);
	void Update(float deltatime);
	void CameraMove(float deltatime);
	void CameraRotation();
	void CalculateCamera();
	VPMath::Matrix GetView() const { return m_view; }
	VPMath::Matrix GetProj() const { return m_proj; }
	

private:
	float m_sensitivity= 1.f/360.f;


	float m_nearZ=0;
	float m_farZ=0;
	VPMath::Matrix m_view;
	VPMath::Matrix m_proj;

	float m_FOV=0;
	float m_DeflaultSpeed = 10;
	float m_PowerSpeed = 50;
	float m_ratio=1;
	bool m_PressedShift=false;
	VPMath::Vector3 m_Transform{};
	VPMath::Quaternion m_Quaternion=VPMath::Quaternion::Identity;
	VPMath::Vector3 m_FrontVector = {};
	VPMath::Vector3 m_UpVector = {};
	VPMath::Vector3 m_RightVector = {};

};

