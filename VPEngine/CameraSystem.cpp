#include "pch.h"
#include "Components.h"
#include "CameraSystem.h"
#include "../VPGraphics/IGraphics.h"

CameraSystem::CameraSystem(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSetMainCamera",CreateSubscriber(&CameraSystem::OnSetMainCamera));
	EventManager::GetInstance().Subscribe("OnResize", CreateSubscriber(&CameraSystem::OnResize));
}

void CameraSystem::LateUpdate(float deltaTime)
{
	bool IsMainCameraExist = false;
	///CameraComponent를 순회하면서 Main카메라에 해당하는 컴포넌틑를 찾아 찾을시 순회를 그만둔다.
	for (CameraComponent& cameracomp : COMPITER(CameraComponent))
	{
		if (cameracomp.IsMain != true)
			continue;
		IsMainCameraExist = true;

		CameraCalculation(cameracomp);
		m_Graphics->SetCamera(cameracomp.View, cameracomp.Proj, cameracomp.OrthoProj);

		///그런다음 for문 종료하기!
		break;
	}

	if (!IsMainCameraExist)
	{
		///존재하지 않는다면, 0,0,0에 해당하는 카메라 정보값을 건네주기!
		//기본 카메라 같은거 만들어 두면 좋을 듯?
		//Graphics::Interface::SetCamera(cameracomp.View, cameracomp.Proj);

	}
}

void CameraSystem::OnResize(std::any hwnd)
{
	auto tempHwnd = std::any_cast<HWND>(hwnd);
	RECT tempsize{};
	GetClientRect(tempHwnd, &tempsize);

	m_Width = static_cast<float>(tempsize.right - tempsize.left);
	m_Height = static_cast<float>(tempsize.bottom - tempsize.top);
}


void CameraSystem::OnSetMainCamera(std::any data)
{

	try
	{
		int mainCameraCount = 0;						//방어코드! 해당 이름에 해당하는 카메라수가 1개가 아닐 때 assert발생!
		if (data.type() != typeid(std::string))
		{
			// 데이터 타입이 std::string이 아니면 로그를 출력하고 함수를 종료합니다.
			VP_ASSERT(false, "data에 std::string이 아닌  다른 자료형이 들어갔습니다.");
			return;
		}
		std::string entityName = std::any_cast<std::string> (data);	///오브젝트 이름으로 찾을까??

		for (CameraComponent& cameracomp : COMPITER(CameraComponent))
		{
			cameracomp.IsMain = false;
			if (cameracomp.GetComponent<IDComponent>()->Name != entityName)
				continue;
			mainCameraCount++;
			cameracomp.IsMain = true;
		}
		if (mainCameraCount != 1)
			VP_ASSERT(false, "해당 이름에 해당하는 카메라가 1개가 아닙니다!");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast에 실패하였습니다.");  // Wrong data type assertion
	}
}


void CameraSystem::CameraCalculation(CameraComponent& mainCamera)
{
	float ratio = m_Width / m_Height;
	if (mainCamera.NearZ < 1)
	{
		mainCamera.NearZ = 1;
	}
	if (mainCamera.FarZ <= mainCamera.NearZ)
	{
		mainCamera.FarZ = mainCamera.NearZ + 1;
	}


	TransformComponent* cameraTransform = mainCamera.GetComponent<TransformComponent>();
	// To get view matrix, we need to invert the world matrix
	VPMath::Vector3 eye = cameraTransform->World_Location;
	VPMath::Vector3 target = eye + cameraTransform->FrontVector; // Assuming the camera looks along the FrontVector
	VPMath::Vector3 up = cameraTransform->UpVector;
	mainCamera.View = VPMath::Matrix::CreateLookAt_LH(eye, target, up);
	// Calculate projection matrix
	// Assuming m_ratio, m_FOV, NearZ, and mainCamera.FarZ are properly set
	mainCamera.Proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(mainCamera.FOV, ratio, mainCamera.NearZ, mainCamera.FarZ);
	mainCamera.OrthoProj = VPMath::Matrix::CreateOrthographic_LH(m_Width, m_Height, mainCamera.NearZ, mainCamera.FarZ);
}

void CameraSystem::RenderUpdate(float deltaTime)
{
}

