#include "pch.h"
#include "EditorCamera.h"
#include "directxtk\SimpleMath.h"
#include <InputManager.h>
#include "HierarchySystem.h"
#include "SceneManager.h"
float WrapAngle(float angle)
{
	while (angle > 360.0f)
		angle -= 360.0f;
	while (angle < -360.0f)
		angle += 360.0f;
	return angle;
}


EditorCamera::EditorCamera(SceneManager* sceneManager) :m_SceneManager{ sceneManager }
{
	Initialize();
}

void EditorCamera::Initialize()
{
	m_Width = 16;
	m_Height = 9;
	m_ratio = { (float)m_Width/ (float)m_Height };

	m_view = VPMath::Matrix::Identity;
	m_proj = VPMath::Matrix::Identity;
	m_FOV = 0.25f * 3.14f;
	m_nearZ = 1.f;
	m_farZ = 1000;
	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(m_FOV, m_ratio, m_nearZ, m_farZ);

}

void EditorCamera::Update(float deltatime)
{
	CameraMove(deltatime);
	CameraRotation();
	DoubleClicked(deltatime);
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
		moveWay = m_ShiftSpeed * deltatime * MoveDirection;
	else
		moveWay = m_DeflaultSpeed * deltatime * MoveDirection;

	m_Location += moveWay;
}


void EditorCamera::CameraRotation()
{
	if (InputManager::GetInstance().GetKey(KEY::RBUTTON))
	{
		VPMath::Vector2 DeltaCurpos = InputManager::GetInstance().GetDeltaCurPos();
		float yaw = DeltaCurpos.x * m_sensitivity;
		float pitch = DeltaCurpos.y * m_sensitivity;

		// Update rotation angles
		m_Rotation.y += yaw; // Yaw affects the y-axis
		m_Rotation.x -= pitch; // Pitch affects the x-axis

		// Clamp pitch to avoid gimbal lock
		if (m_Rotation.x > m_maxPitch)
			m_Rotation.x = m_maxPitch;
		if (m_Rotation.x < -m_maxPitch)
			m_Rotation.x = -m_maxPitch;
	}
}



void EditorCamera::CalculateCamera()
{
	if (m_nearZ < 1)
	{
		m_nearZ = 1;
	}
	if (m_farZ <= m_nearZ)
	{
		m_farZ = m_nearZ + 1;
	}

	if (m_Width <= 0)
	{
		m_Width = 1;
	}
	if (m_Height <= 0)
	{
		m_Height = 1;
	}
	CalculateCameraTransform();

	m_ratio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	if (m_ratio < 0.00001f)
	{
		m_ratio = 0.00002f;
	}




	// Update view matrix
	// To get view matrix, we need to invert the world matrix
	VPMath::Vector3 eye = m_Location;
	VPMath::Vector3 target = eye + m_FrontVector; // Assuming the camera looks along the FrontVector
	VPMath::Vector3 up = m_UpVector;
	m_view = VPMath::Matrix::CreateLookAt_LH(eye, target, up);

	// Calculate projection matrix
	// Assuming m_ratio, m_FOV, m_nearZ, and m_farZ are properly set
	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(m_FOV, m_ratio, m_nearZ, m_farZ);
}

void EditorCamera::CalculateCameraTransform()
{

	// Lambda function to wrap the angle
	auto wrapAngle = [](float angle) -> float {
		while (angle <= -360.0f) angle += 720.0f;
		while (angle > 360.0f) angle -= 720.0f;
		return angle;
		};

	m_Rotation.x = wrapAngle(m_Rotation.x);
	m_Rotation.y = wrapAngle(m_Rotation.y);
	m_Rotation.z = wrapAngle(m_Rotation.z);

	m_Quaternion = VPMath::Quaternion::CreateFromYawPitchRoll(
		VPMath::XMConvertToRadians(m_Rotation.y),
		VPMath::XMConvertToRadians(m_Rotation.x),
		VPMath::XMConvertToRadians(m_Rotation.z));

	m_Transform = VPMath::Matrix::CreateScale(m_Scale) *
		VPMath::Matrix::CreateFromQuaternion(m_Quaternion) *
		VPMath::Matrix::CreateTranslation(m_Location);

	m_FrontVector = m_Transform.Forward();
	m_RightVector = m_Transform.Left();
	m_UpVector = m_Transform.Up();


}

void EditorCamera::DoubleClicked(float deltatime)
{
	if (!m_SceneManager->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;
	auto transform = m_SceneManager->GetComponent<TransformComponent>( HierarchySystem::m_SelectedEntityID);

	if (!HierarchySystem::IsItemDoubleClicked)
	{
		m_LerpTime = 0;
	m_LerpStartPos= m_Location;
	m_LerpEndPos.x = transform->World_Location.x - 30 * m_FrontVector.x;
	m_LerpEndPos.y = transform->World_Location.y - 30 * m_FrontVector.y;
	m_LerpEndPos.z = transform->World_Location.z - 30 * m_FrontVector.z;
	}
	else
	{
		m_LerpTime += deltatime;
		if (m_LerpTime > m_LerpEndTime)
		{
			HierarchySystem::IsItemDoubleClicked = false;
			m_LerpTime =m_LerpEndTime;
		}
		m_Location = VPMath::Vector3::Lerp(m_LerpStartPos, m_LerpEndPos, (m_LerpTime / m_LerpEndTime));
	
	}

}

void EditorCamera::ImGuiRender()
{
	
	ImGui::Begin("EditorCamera Setting");
	{
		float dragBoxWidth = 200.0f; // 예를 들어 200 픽셀 너비

		// 다음 아이템의 너비 설정
		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat3("Transform", &m_Location.x, 1.f, -FLT_MAX, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat3("Rotation", &m_Rotation.x, 1.f, -FLT_MAX, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat("FOV", &m_FOV, .001f, -FLT_MAX, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat("NearZ", &m_nearZ, 1.0f, 1, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat("FarZ", &m_farZ, 1.0f, 1, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat("Speed", &m_DeflaultSpeed, 1.f, -FLT_MAX, FLT_MAX);

		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragFloat("MaxSpeed", &m_ShiftSpeed, 1.f, -FLT_MAX, FLT_MAX);


		int CameraSize[2] = { (int)m_Width,(int)m_Height };
		ImGui::SetNextItemWidth(dragBoxWidth);
		ImGui::DragInt2("Width/Height", CameraSize, 1, 1, INT_MAX);

		ImGui::SameLine();

		if (CameraSize[0] <=0)
		{
			CameraSize[0] = 1;
		}
		if (CameraSize[1] <= 0)
		{
			CameraSize[1] = 1;
		}

		m_Width = CameraSize[0];
		m_Height = CameraSize[1];

	}

	ImGui::End();
}
