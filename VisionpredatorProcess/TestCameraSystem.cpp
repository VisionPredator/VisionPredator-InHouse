#include "pch.h"
#include "TestCameraSystem.h"
#include "Managers.h"
#include "Components.h"

TestCameraSystem::TestCameraSystem(SceneManager* sceneManager) :System(sceneManager)
{
}
void TestCameraSystem::Update(float deltaTime)
{
	for (CameraComponent& Camera :COMPITER(CameraComponent))
	{

		if (!Camera.IsMain)
			continue;
		if (!Camera.GetComponent<IdentityComponent>())
			continue;
		if (Camera.GetComponent<IdentityComponent>()->UUID != "TestCamera")
			continue;

		TransformComponent* cameraTransform = Camera.GetComponent<TransformComponent>();

		VPMath::Vector3 moveDirection{};
		VPMath::Vector3 MoveWay{};

		if (INPUTKEY(KEY::W))
			moveDirection+=cameraTransform->FrontVector;
		if (INPUTKEY(KEY::S))
			moveDirection -= cameraTransform->FrontVector;
		if (INPUTKEY(KEY::A))
			moveDirection -= cameraTransform->RightVector;
		if (INPUTKEY(KEY::D))
			moveDirection += cameraTransform->RightVector;
		if (INPUTKEY(KEY::Q))
			moveDirection.y-=1;
		if (INPUTKEY(KEY::E))
			moveDirection.y += 1;


		moveDirection.Normalize();
		cameraTransform->Local_Location+=  moveSpeed* moveDirection* deltaTime;

		if (InputManager::GetInstance().GetKey(KEY::RBUTTON))
		{
			VPMath::Vector2 DeltaCurpos = InputManager::GetInstance().GetDeltaCurPos();
			float yaw = DeltaCurpos.x * 0.1f;
			float pitch = DeltaCurpos.y * 0.1f;

			// Update rotation angles
			cameraTransform->Local_Rotation.y += yaw; // Yaw affects the y-axis
			cameraTransform->Local_Rotation.x -= pitch; // Pitch affects the x-axis

			// Clamp pitch to avoid gimbal lock
			if (cameraTransform->Local_Rotation.x > 89.9f)
				cameraTransform->Local_Rotation.x = 89.9f;
			if (cameraTransform->Local_Rotation.x < -89.9f)
				cameraTransform->Local_Rotation.x = -89.9f;
		}
		break;
	}


}

void TestCameraSystem::FixedUpdate(float deltaTime)
{
}
