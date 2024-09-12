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
		Calculate_FSM(comp);
		Action_FSM(comp);

	}
}



void PlayerSystem::PhysicsUpdate(float deltaTime)
{

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
#pragma region FSM Calculate

void PlayerSystem::Calculate_FSM(PlayerComponent& playercomp)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		Calculate_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::MOVE:
		Calculate_Move(playercomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		Calculate_Attack(playercomp);
		break;
	case VisPred::Game::EFSM::DIE:
		Calculate_Die(playercomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
		Calculate_Destroy(playercomp);
		break;
	default:
		break;
	}
}

void PlayerSystem::Calculate_Idle(PlayerComponent& playercomp)
{
	if (INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::MOVE;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;
	}
}

void PlayerSystem::Calculate_Die(PlayerComponent& playercomp)
{
}

void PlayerSystem::Calculate_Attack(PlayerComponent& playercomp)
{
}

void PlayerSystem::Calculate_Move(PlayerComponent& playercomp)
{
	if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;
	}
}

void PlayerSystem::Calculate_Destroy(PlayerComponent& playercomp)
{
}

#pragma endregion

#pragma region FSM Action
void PlayerSystem::Action_FSM(PlayerComponent& playercomp)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		Action_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::MOVE:
		Action_Move(playercomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		Action_Attack(playercomp);
		break;
	case VisPred::Game::EFSM::DIE:
		Action_Die(playercomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
		Action_Destroy(playercomp);
		break;
	default:
		break;
	}
}

void PlayerSystem::Action_Idle(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	Move_Rotation(playercomp, transfomcomp);

}

void PlayerSystem::Action_Move(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Move_Walk(transfomcomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
	Move_Jump(transfomcomp, Controller);
	Move_Slide(playercomp);
}

void PlayerSystem::Action_Attack(PlayerComponent& playercomp)
{
}

void PlayerSystem::Action_Die(PlayerComponent& playercomp)
{
}

void PlayerSystem::Action_Destroy(PlayerComponent& playercomp)
{
}
#pragma endregion

#pragma region Shoot Logic
void PlayerSystem::Shoot_Style(PlayerComponent& playercomp)
{
}

void PlayerSystem::Shoot_Pistol(PlayerComponent& playercomp)
{
}

void PlayerSystem::Shoot_ShotGun(PlayerComponent& playercomp)
{
}
void PlayerSystem::Shoot_Rifle(PlayerComponent& playercomp)
{
}

#pragma endregion

#pragma region Move Logic

void PlayerSystem::Move_Walk(const TransformComponent& transformcomp, ControllerComponent& controllercomp)
{							 
	controllercomp.InputDir = {};
	if (INPUTKEY(KEYBOARDKEY::W))
		controllercomp.InputDir += transformcomp.FrontVector;
	if (INPUTKEY(KEYBOARDKEY::S))
		controllercomp.InputDir -= transformcomp.FrontVector;
	if (INPUTKEY(KEYBOARDKEY::A))
		controllercomp.InputDir -= transformcomp.RightVector;
	if (INPUTKEY(KEYBOARDKEY::D))
		controllercomp.InputDir += transformcomp.RightVector;
}

void PlayerSystem::Move_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp)
{
	// 컴포넌트 접근을 캐싱
	// 입력 매니저에서 마우스 델타 값을 미리 가져옴
	int deltaCurposX = InputManager::GetInstance().GetMouseDeltaX();
	int deltaCurposY = InputManager::GetInstance().GetMouseDeltaY();
	float sensitivity = playercomp.Sencitive;
	// 플레이어의 회전 업데이트
	float yaw = deltaCurposX * sensitivity;
	VPMath::Vector3 playerRotation = transformcomp.World_Rotation;
	playerRotation.y += yaw;
	transformcomp.SetWorldRotation(playerRotation);
	// 자식 컴포넌트가 있는 경우에만 처리
	if (!playercomp.HasComponent<Children>())
		return;
	auto children = playercomp.GetComponent<Children>();
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

void PlayerSystem::Move_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp)
{
	controllercomp.InputDir.y = {};
	if (INPUTKEY(KEYBOARDKEY::SPACE))
		controllercomp.InputDir += transformcomp.UpVector;
}

void PlayerSystem::Move_Slide(PlayerComponent& playercomp)
{
}
#pragma endregion
