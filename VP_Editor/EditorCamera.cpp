#include "pch.h"
#include "EditorCamera.h"
#include <InputManager.h>
#include "HierarchySystem.h"
#include "../VPCommon/EventManager.h"
#include "SceneManager.h"

float WrapAngle(float angle)
{
	while (angle > 360.0f)
		angle -= 360.0f;
	while (angle < -360.0f)
		angle += 360.0f;
	return angle;
}


EditorCamera::EditorCamera(std::shared_ptr<SceneManager> sceneManager) :m_SceneManager{ sceneManager }
{
	Initialize();
}

void EditorCamera::Initialize()
{
	m_Width = 16;
	m_Height = 9;
	m_ratio = static_cast<float>(m_Width) / static_cast<float>(m_Height);

	m_view = VPMath::Matrix::Identity;
	m_proj = VPMath::Matrix::Identity;
	m_FOV = 0.25f * 3.14f;
	m_nearZ = 1.f;
	m_farZ = 1000;
	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(m_FOV, m_ratio, m_nearZ, m_farZ);

	// SUMIN_
	m_orthoProj = VPMath::Matrix::CreateOrthographic_LH(m_Width, m_Height, m_nearZ, m_farZ);
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
	if (!InputManager::GetInstance().GetKey(MOUSEKEY::RBUTTON))
		return;

	VPMath::Vector3 moveDirection = {};

	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::W))
		moveDirection += m_FrontVector;
	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::S))
		moveDirection -= m_FrontVector;
	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::A))
		moveDirection -= m_RightVector;
	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::D))
		moveDirection += m_RightVector;
	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::Q))
		moveDirection.y -= 1;
	if (InputManager::GetInstance().GetKey(KEYBOARDKEY::E))
		moveDirection.y += 1;

	if (moveDirection.Length() < 0.5f)
		return;

	moveDirection.Normalize();
	VPMath::Vector3 moveWay = (m_PressedShift ? m_ShiftSpeed : m_DeflaultSpeed) * deltatime * moveDirection;

	m_Location += moveWay;

	if (InputManager::GetInstance().GetKeyDown(KEYBOARDKEY::LSHIFT))
		m_PressedShift = true;
	else if (InputManager::GetInstance().GetKeyUp(KEYBOARDKEY::LSHIFT))
		m_PressedShift = false;
}

void EditorCamera::CameraRotation()
{
	if (InputManager::GetInstance().GetKey(MOUSEKEY::RBUTTON))
	{
		int deltaCurposx = InputManager::GetInstance().GetMouseDeltaX();
		int deltaCurposy = InputManager::GetInstance().GetMouseDeltaY();
		float yaw = deltaCurposx * m_sensitivity;
		float pitch = deltaCurposy * m_sensitivity;

		m_Rotation.y += yaw;
		m_Rotation.x += pitch;

		m_Rotation.x = std::clamp(m_Rotation.x, -m_maxPitch, m_maxPitch);
	}
}




void EditorCamera::CalculateCamera()
{
	if (m_nearZ < 1.0f)
		m_nearZ = 1.0f;

	if (m_farZ < m_nearZ + 1.0f)
		m_farZ = m_nearZ + 1.0f;

	if (m_Width < 1)
		m_Width = 1;

	if (m_Height < 1)
		m_Height = 1;

	CalculateCameraTransform();

	m_ratio = static_cast<float>(m_Width) / static_cast<float>(m_Height);
	if (m_ratio < 0.00002f)
		m_ratio = 0.00002f;

	VPMath::Vector3 eye = m_Location;
	VPMath::Vector3 target = eye + m_FrontVector;
	VPMath::Vector3 up = m_UpVector;
	m_view = VPMath::Matrix::CreateLookAt_LH(eye, target, up);

	m_proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(m_FOV, m_ratio, m_nearZ, m_farZ);
	m_orthoProj = VPMath::Matrix::CreateOrthographic_LH(m_Width, m_Height, m_nearZ, m_farZ);

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

	m_FrontVector = -m_Transform.Forward();
	m_RightVector = m_Transform.Right();
	m_UpVector = m_Transform.Up();

}

void EditorCamera::DoubleClicked(float deltatime)
{
	auto sceneManager = m_SceneManager.lock();
	if (!sceneManager || !sceneManager->HasEntity(HierarchySystem::m_SelectedEntityID))
		return;

	auto transform = sceneManager->GetComponent<TransformComponent>(HierarchySystem::m_SelectedEntityID);

	if (!HierarchySystem::IsItemDoubleClicked)
	{
		m_LerpTime = 0;
		m_LerpStartPos = m_Location;
		m_LerpEndPos = transform->World_Location - 30 * m_FrontVector;
	}
	else
	{
		m_LerpTime += deltatime;
		if (m_LerpTime > m_LerpEndTime)
		{
			HierarchySystem::IsItemDoubleClicked = false;
			m_LerpTime = m_LerpEndTime;
		}
		m_Location = VPMath::Vector3::Lerp(m_LerpStartPos, m_LerpEndPos, m_LerpTime / m_LerpEndTime);
	}
}

void EditorCamera::OnReSize(std::any hwnd)
{
	auto tempHwnd = std::any_cast<HWND>(hwnd);
	RECT tempsize{};
	GetClientRect(tempHwnd, &tempsize);

	m_Width = tempsize.right - tempsize.left;
	m_Height = tempsize.bottom - tempsize.top;
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
	}

	ImGui::End();
}
