#include "pch.h"
#include "TestCameraSystem.h"
#include "Managers.h"
#include "Components.h"

TestCameraSystem::TestCameraSystem(std::shared_ptr<SceneManager> sceneManager) :System(sceneManager)
{
}
void TestCameraSystem::Update(float deltaTime)
{
	for (CameraComponent& Camera : COMPITER(CameraComponent))
	{

		if (!Camera.IsMain)
			continue;
		if (!Camera.HasComponent<IdentityComponent>())
			continue;
		if (Camera.HasComponent<IdentityComponent>())
			if (Camera.GetComponent<IdentityComponent>()->UUID != "TestCamera")
			continue;

		TransformComponent* cameraTransform = Camera.GetComponent<TransformComponent>();

		VPMath::Vector3 moveDirection{};
		VPMath::Vector3 MoveWay{};

		if (INPUTKEY(KEYBOARDKEY::W))
			moveDirection += cameraTransform->FrontVector;
		if (INPUTKEY(KEYBOARDKEY::S))
			moveDirection -= cameraTransform->FrontVector;
		if (INPUTKEY(KEYBOARDKEY::A))
			moveDirection -= cameraTransform->RightVector;
		if (INPUTKEY(KEYBOARDKEY::D))
			moveDirection += cameraTransform->RightVector;
		if (INPUTKEY(KEYBOARDKEY::Q))
			moveDirection.y -= 1;
		if (INPUTKEY(KEYBOARDKEY::E))
			moveDirection.y += 1;

		moveDirection.Normalize();
		cameraTransform->World_Location += moveSpeed * moveDirection * deltaTime;

		if (INPUTKEY(MOUSEKEY::RBUTTON))
		{
			INT DeltaCurposX = InputManager::GetInstance().GetMouseDeltaX();
			INT DeltaCurposY = InputManager::GetInstance().GetMouseDeltaY();
			float yaw = DeltaCurposX * 0.1f;
			float pitch = DeltaCurposY * 0.1f;

			// Update rotation angles
			cameraTransform->World_Rotation.y += yaw; // Yaw affects the y-axis
			cameraTransform->World_Rotation.x += pitch; // Pitch affects the x-axis

			// Clamp pitch to avoid gimbal lock
			if (cameraTransform->World_Rotation.x > 89.9f)
				cameraTransform->World_Rotation.x = 89.9f;
			if (cameraTransform->World_Rotation.x < -89.9f)
				cameraTransform->World_Rotation.x = -89.9f;
		}
		break;
	}

}

