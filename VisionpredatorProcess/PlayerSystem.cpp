#include "pch.h"
#include "PlayerSystem.h"
PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void PlayerSystem::Update(float deltaTime)
{
	for (PlayerComponent& comp: COMPITER(PlayerComponent))
	{
		TransformComponent* TransformComp = comp.GetComponent<TransformComponent>();

		if (comp.HasComponent<ControllerComponent>())
		{
			PlayerLocation(TransformComp);

		}
		PlayerRotation(TransformComp);
		PlayerShoot(comp);
	}
}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{

}

void PlayerSystem::PlayerLocation(TransformComponent* comp)
{
	ControllerComponent* controllercomp = comp->GetComponent<ControllerComponent>();
	controllercomp->InputDir = {};
	if (INPUTKEY(KEYBOARDKEY::W))
	{
		controllercomp->InputDir += comp->FrontVector;
	}
	if (INPUTKEY(KEYBOARDKEY::S))
	{
		controllercomp->InputDir -= comp->FrontVector;

	}
	if (INPUTKEY(KEYBOARDKEY::A))
	{
		controllercomp->InputDir -= comp->RightVector;
	}
	if (INPUTKEY(KEYBOARDKEY::D))
	{
		controllercomp->InputDir += comp->RightVector;
	}
}

void PlayerSystem::PlayerRotation(TransformComponent* comp)
{
	// 컴포넌트 접근을 캐싱
	auto player = comp->GetComponent<PlayerComponent>();
	// 입력 매니저에서 마우스 델타 값을 미리 가져옴
	int deltaCurposX = InputManager::GetInstance().GetMouseDeltaX();
	int deltaCurposY = InputManager::GetInstance().GetMouseDeltaY();
	float sensitivity = player->Sencitive;
	// 플레이어의 회전 업데이트
	float yaw = deltaCurposX * sensitivity;
	VPMath::Vector3 playerRotation = comp->World_Rotation;
	playerRotation.y += yaw;
	comp->SetWorldRotation(playerRotation);

	// 자식 컴포넌트가 있는 경우에만 처리
	if (!player->HasComponent<Children>())
		return;
	auto children = player->GetComponent<Children>();
	for (uint32_t childID : children->ChildrenID)
	{
		if (!GetSceneManager()->HasComponent<CameraComponent>(childID))
			continue;
		auto camera = GetSceneManager()->GetComponent<CameraComponent>(childID);
		TransformComponent* cameratransform = camera->GetComponent<TransformComponent>();
		// 카메라 회전 업데이트
		float pitch = deltaCurposY * sensitivity;
		VPMath::Vector3 cameraRotation = cameratransform->Local_Rotation;
		cameraRotation.x += pitch;
		cameraRotation.x = std::clamp(cameraRotation.x, -89.9f, 89.9f);
		cameratransform->SetLocalRotation(cameraRotation);
	}
}



void PlayerSystem::PlayerShoot(PlayerComponent& comp)
{
	const TransformComponent& transform = *comp.GetComponent<TransformComponent>();
	if (INPUTKEYDOWN(MOUSEKEY::LBUTTON))
	{
		if (!comp.HasGun)
			return;
		auto posEntity = GetSceneManager()->GetChildEntityByName(comp.GetEntityID(), comp.FirPosition);
		if (!posEntity)
			return;
		auto tempTransform = posEntity->GetComponent<TransformComponent>();
		auto temppos = tempTransform->World_Location;
		auto temprotatin = tempTransform->World_Rotation;
		VPMath::Vector3 tempscale = { 1,1,1 };
		m_SceneManager.lock()->SpawnPrefab("../Data/Prefab/cube.prefab", temppos, temprotatin, tempscale);
	}
}
