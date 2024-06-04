#include "pch.h"
#include "EditorCamera.h"
#include "directxtk\SimpleMath.h"
#include <InputManager.h>
EditorCamera::EditorCamera()
{
	Initialize((9.f / 16.f));
}

void EditorCamera::Initialize(double ratio)
{
	m_ratio = { 16.f / 9.f };

	m_view = VPMath::Matrix::Identity;
	m_proj = VPMath::Matrix::Identity;
	m_FOV = 0.25f * 3.14f;
	m_nearZ = 1.f;
	m_farZ = 1000;
	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView(m_FOV, m_ratio, m_nearZ, m_farZ);

}

void EditorCamera::Update(float deltatime)
{
	CameraMove(deltatime);
	CameraRotation();
	CalculateCamera();
}

void EditorCamera::CameraMove(float deltatime)
{
	VPMath::Vector3 MoveDirection = {};

	if (InputManager::GetInstance().GetKey(KEY::RBUTTON))
	{
	if (InputManager::GetInstance().GetKey(KEY::W))
		MoveDirection += m_FrontVector;
	if (InputManager::GetInstance().GetKey(KEY::S))
		MoveDirection -= m_FrontVector;
	if (InputManager::GetInstance().GetKey(KEY::A))
		MoveDirection -= m_RightVector;
	if (InputManager::GetInstance().GetKey(KEY::D))
		MoveDirection += m_RightVector;
	if (InputManager::GetInstance().GetKey(KEY::Q))
		MoveDirection.y -= 1;
	if (InputManager::GetInstance().GetKey(KEY::E))
		MoveDirection.y += 1;

	if (InputManager::GetInstance().GetKeyDown(KEY::LSHFIT))
		m_PressedShift = true;
	else if (InputManager::GetInstance().GetKeyUp(KEY::LSHFIT))
		m_PressedShift = false;
	}

	if (MoveDirection.Length() < 0.5)
		return;

	MoveDirection.Normalize();
	VPMath::Vector3 moveWay{};
	if (m_PressedShift)
		moveWay = m_PowerSpeed * deltatime * MoveDirection;
	else
		moveWay = m_DeflaultSpeed * deltatime * MoveDirection;

	m_Transform += moveWay;
}

void EditorCamera::CameraRotation()
{
	if (InputManager::GetInstance().GetKey(KEY::RBUTTON))
	{

	VPMath::Vector2 DeltaCurpos = InputManager::GetInstance().GetDeltaCurPos();
	VPMath::Quaternion quater = {};
	float yaw = DeltaCurpos.x * m_sensitivity;
	float pitch = DeltaCurpos.y * m_sensitivity;

	// Create quaternions for yaw and pitch rotations
	VPMath::Quaternion yawQuat = VPMath::Quaternion::CreateFromAxisAngle(VPMath::Vector3(0.0f, 1.0f, 0.0f), yaw);
	VPMath::Quaternion pitchQuat = VPMath::Quaternion::CreateFromAxisAngle(VPMath::Vector3(1.0f, 0.0f, 0.0f), pitch);

	// Update camera orientation
     	m_Quaternion = pitchQuat * m_Quaternion; // Apply yaw rotation
	m_Quaternion = m_Quaternion * yawQuat; // Apply pitch rotation
	m_Quaternion.Normalize();
	}
}

void EditorCamera::CalculateCamera()
{
	// Create rotation matrix from quaternion
	VPMath::Matrix rotationMatrix = VPMath::Matrix::CreateFromQuaternion(m_Quaternion);



	// Extract front and up vectors from rotation matrix
	m_FrontVector = VPMath::Vector3(rotationMatrix._31, rotationMatrix._32, rotationMatrix._33);
	m_UpVector = VPMath::Vector3(rotationMatrix._21, rotationMatrix._22, rotationMatrix._23);
	m_RightVector= m_UpVector.Cross(m_FrontVector);
	m_FrontVector.Normalize();
	m_UpVector.Normalize();
	m_RightVector.Normalize();
	// Update view matrix
	// To get view matrix, we need to invert the world matrix
	VPMath::Vector3 eye = m_Transform;
	VPMath::Vector3 target = eye + m_FrontVector; // Assuming the camera looks along the FrontVector
	VPMath::Vector3 up = m_UpVector;
	m_view = VPMath::Matrix::CreateLookAt_LH(eye, target, up);
	// Calculate projection matrix
	// Assuming m_ratio, m_FOV, m_nearZ, and m_farZ are properly set
	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(m_FOV, m_ratio, m_nearZ, m_farZ);

	//// Calculate world-view-projection matrix
	//m_worldviewproj = m_world * m_view * m_proj;
}


///
///	SetCameraSetting
/// 