#include "pch.h"
#include "Components.h"
#include "CameraSystem.h"
#include "../VPGraphics/IGraphics.h"

CameraSystem::CameraSystem(SceneManager* sceneManager) :System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnSetMainCamera",CreateSubscriber(&CameraSystem::OnSetMainCamera));

}


void CameraSystem::OnSetMainCamera(std::any data)
{

	try
	{
		int mainCameraCount = 0;						//����ڵ�! �ش� �̸��� �ش��ϴ� ī�޶���� 1���� �ƴ� �� assert�߻�!
		if (data.type() != typeid(std::string))
		{
			// ������ Ÿ���� std::string�� �ƴϸ� �α׸� ����ϰ� �Լ��� �����մϴ�.
			VP_ASSERT(false, "data�� std::string�� �ƴ�  �ٸ� �ڷ����� �����ϴ�.");
			return;
		}
		std::string entityName = std::any_cast<std::string> (data);	///������Ʈ �̸����� ã����??

		for (CameraComponent& cameracomp : COMPITER(CameraComponent))
		{
			cameracomp.IsMain = false;

			if (cameracomp.GetComponent<IDComponent>()->Name != entityName)
				continue;
			mainCameraCount++;
			cameracomp.IsMain = true;
		}
		if (mainCameraCount != 1)
			VP_ASSERT(false, "�ش� �̸��� �ش��ϴ� ī�޶� 1���� �ƴմϴ�!");
	}
	catch (const std::bad_any_cast&)
	{
		VP_ASSERT(false, "std::any_cast�� �����Ͽ����ϴ�.");  // Wrong data type assertion
	}
}

void CameraSystem::FixedUpdate(float deltaTime)
{

}

void CameraSystem::CameraCalculation(CameraComponent& mainCamera)
{

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
	mainCamera.Proj = VPMath::Matrix::CreatePerspectiveFieldOfView_LH(mainCamera.FOV, mainCamera.Ratio, mainCamera.NearZ, mainCamera.FarZ);
}

void CameraSystem::RenderUpdate(float deltaTime)
{
}

void CameraSystem::Update(float deltaTime)
{
	bool IsMainCameraExist = false;
	///CameraComponent�� ��ȸ�ϸ鼭 Mainī�޶� �ش��ϴ� �����ͺz�� ã�� ã���� ��ȸ�� �׸��д�.
	for (CameraComponent& cameracomp : COMPITER(CameraComponent))
	{
		if (cameracomp.IsMain != true)
			continue;
		IsMainCameraExist = true;

		CameraCalculation(cameracomp);
		m_Graphics->SetCamera(cameracomp.View, cameracomp.Proj);

		///�׷����� for�� �����ϱ�!
		break;
	}

	if (!IsMainCameraExist)
	{
		///�������� �ʴ´ٸ�, 0,0,0�� �ش��ϴ� ī�޶� �������� �ǳ��ֱ�!
		//�⺻ ī�޶� ������ ����� �θ� ���� ��?
		//Graphics::Interface::SetCamera(cameracomp.View, cameracomp.Proj);

	}
}

