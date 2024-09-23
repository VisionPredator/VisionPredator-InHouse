#include "pch.h"
#include "PlayerSystem.h"
#include <iostream>
PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void PlayerSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		Calculate_FSM(playercomp);
		Action_FSM(playercomp,deltaTime);
		RaycastTest(playercomp);
	}
}

void PlayerSystem::RaycastTest(PlayerComponent& playercomp)
{
	if (INPUTKEYDOWN(KEYBOARDKEY::R))
	{
		auto posEntity = GetSceneManager()->GetChildEntityByName(playercomp.GetEntityID(), "PlayerCamera");
		if (!posEntity)
			return;
		if (!posEntity->HasComponent<CameraComponent>())
			return;
		auto cameratransform = posEntity->GetComponent<TransformComponent>();
		auto front = cameratransform->FrontVector;
		std::cout << m_PhysicsEngine->RaycastToHitActor(playercomp.GetEntityID(), front, 500);
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
		auto posEntity = GetSceneManager()->GetChildEntityByName(comp.GetEntityID(), comp.FirePosition);
		if (!posEntity)
			return;
		auto tempTransform = posEntity->GetComponent<TransformComponent>();
		auto temppos = tempTransform->World_Location;
		auto temprotatin = tempTransform->World_Rotation;
		VPMath::Vector3 tempscale = { 1,1,1 };
		m_SceneManager.lock()->SpawnPrefab("../Data/Prefab/cube.prefab", temppos, temprotatin, tempscale);
	}
}
void PlayerSystem::UpdateCharDataToController(PlayerComponent& playercomp)
{
	ControllerComponent& controllercomp=  *playercomp.GetComponent<ControllerComponent>();
	controllercomp.Acceleration= playercomp.Accel;
	controllercomp.MaxSpeed = playercomp.WalkSpeed;
	controllercomp.JumpSpeed = playercomp.JumpFoce;
	controllercomp.StaticFriction = playercomp.StaticFriction;
	controllercomp.DynamicFriction = playercomp.DynamicFriction;
	controllercomp.JumpXZAcceleration = playercomp.Accel *playercomp.AirControlPercent/100;
	controllercomp.GravityWeight = playercomp.GravityPower* 9.80665;




}
void PlayerSystem::UpdateControllerSize(PlayerComponent& playercomp)
{
	ControllerComponent& controllercomp = *playercomp.GetComponent<ControllerComponent>();
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controllercomp.CapsuleControllerinfo.height, controllercomp.CapsuleControllerinfo.radius);
}
void PlayerSystem::CrouchModeController(PlayerComponent & playercomp)
{
	ControllerComponent& controllercomp = *playercomp.GetComponent<ControllerComponent>();
	controllercomp.Contollerinfo.LocalOffset.y = -playercomp.SitHeightDiff;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controllercomp.CapsuleControllerinfo.radius, playercomp.SitHeight);
}
void PlayerSystem::DefalutModeController(PlayerComponent & playercomp)
{
	ControllerComponent& controllercomp = *playercomp.GetComponent<ControllerComponent>();
	controllercomp.Contollerinfo.LocalOffset.y = 0;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controllercomp.CapsuleControllerinfo.radius, controllercomp.CapsuleControllerinfo.height);
}
void PlayerSystem::DownCamera(PlayerComponent & playercomp,float deltatime)
{
	auto cameraentity = GetSceneManager()->GetChildEntityByName(playercomp.GetEntityID(), "PlayerCamera");
	auto& cameracomp =  *cameraentity->GetComponent<TransformComponent>();
	///카메라와 앉은 위치의 pos 와의 거리가 0.01 보다 작을 때.
	if ((playercomp.SitCameraPos - cameracomp.Local_Location).Length()<0.01f)
	{
		playercomp.SitCameraPos = cameracomp.Local_Location;
		playercomp.CamTransProgress = 0;
	}
	else
	{
		playercomp.CamTransProgress += deltatime;
		VPMath::Vector3 Temp{};
		if (playercomp.CamTransProgress>= playercomp.CamTransDuration)
		{
			playercomp.CamTransProgress = playercomp.CamTransDuration;
		}
		Temp = VPMath::Vector3::Lerp(playercomp.DefalutCameraPos, playercomp.SitCameraPos, (playercomp.CamTransProgress / playercomp.CamTransDuration));
		cameracomp.SetLocalLocation(Temp);
	}
}
void PlayerSystem::UpCamera(PlayerComponent& playercomp, float deltatime)
{
	auto cameraentity = GetSceneManager()->GetChildEntityByName(playercomp.GetEntityID(), "PlayerCamera");
	auto& cameracomp = *cameraentity->GetComponent<TransformComponent>();
	///카메라와 앉은 위치의 pos 와의 거리가 0.01 보다 작을 때.
	if ((playercomp.DefalutCameraPos - cameracomp.Local_Location).Length() < 0.01f)
	{
		playercomp.DefalutCameraPos = cameracomp.Local_Location;
		playercomp.CamTransProgress = 0;
	}
	else
	{
		playercomp.CamTransProgress += deltatime;
		VPMath::Vector3 Temp{};
		if (playercomp.CamTransProgress >= playercomp.CamTransDuration)
		{
			playercomp.CamTransProgress = playercomp.CamTransDuration;
		}
		Temp = VPMath::Vector3::Lerp(playercomp.SitCameraPos, playercomp.DefalutCameraPos, (playercomp.CamTransProgress / playercomp.CamTransDuration));
		cameracomp.SetLocalLocation(Temp);
	}

}

void PlayerSystem::CarmeraPosChange(PlayerComponent& playercomp, float deltatime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::CROUCH:
	case VisPred::Game::EFSM::SLIDE:
	{
		DownCamera(playercomp, deltatime);
	}
	break;
	default:
	{
		UpCamera(playercomp, deltatime);
	}
	break;
	}


}

void PlayerSystem::SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp)
{
	
	if (playercomp.GetEntityID()!= controllercomp.GetEntityID())
		return;
	playercomp.SlideDir = controllercomp.InputDir;
}

#pragma region FSM Calculate

void PlayerSystem::Calculate_FSM(PlayerComponent & playercomp)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		Calculate_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::WALK:
		Calculate_Walk(playercomp);
		break;
	case VisPred::Game::EFSM::RUN:
		Calculate_Run(playercomp);
		break;
	case VisPred::Game::EFSM::JUMP:
		Calculate_Jump(playercomp);
		break;
	case VisPred::Game::EFSM::CROUCH:
		Calculate_Crouch(playercomp);
		break;
	case VisPred::Game::EFSM::SLIDE:
		Calculate_Slide(playercomp);
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
	///뛰기
	if ((INPUTKEYDOWN(KEYBOARDKEY::LSHIFT)||INPUTKEY(KEYBOARDKEY::LSHIFT)) 
		&& INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::RUN;
	}
	///걷기
	else if (INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;
	}
	///슬라이딩
	else if ((INPUTKEYDOWN(KEYBOARDKEY::LSHIFT) || INPUTKEY(KEYBOARDKEY::LSHIFT)) &&
		INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::EFSM::SLIDE;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::EFSM::CROUCH;
	}
	///점프
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE)|| playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;

}

void PlayerSystem::Calculate_Die(PlayerComponent& playercomp)
{

}



void PlayerSystem::Calculate_Attack(PlayerComponent& playercomp)
{
}

void PlayerSystem::Calculate_Walk(PlayerComponent& playercomp)
{
	if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::RUN;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL) || INPUTKEY(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);

		playercomp.CurrentFSM = VisPred::Game::EFSM::CROUCH;
	}
}
void PlayerSystem::Calculate_Run(PlayerComponent& playercomp)
{
	if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
	else if (INPUTKEYUP(KEYBOARDKEY::LSHIFT))
	{
		playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::EFSM::SLIDE;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;
}

void PlayerSystem::Calculate_Crouch(PlayerComponent& playercomp)
{
	if (INPUTKEYUP(KEYBOARDKEY::LCONTROL))
	{
		if (INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			DefalutModeController(playercomp);
			playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;
		}
		else
		{
			DefalutModeController(playercomp);
			playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
		}
	}
}
void PlayerSystem::Calculate_Slide(PlayerComponent& playercomp)
{
	if (playercomp.SlideProgress>= playercomp.SlideDuration)
	{
		playercomp.SlideProgress = 0;
		DefalutModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::EFSM::RUN;

	}


}
void PlayerSystem::Calculate_Jump(PlayerComponent& playercomp)
{
	if (!playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;

}

void PlayerSystem::Calculate_Destroy(PlayerComponent& playercomp)
{
}

#pragma endregion

#pragma region FSM Action
void PlayerSystem::Action_FSM(PlayerComponent& playercomp, float deltaTime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		Action_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::WALK:
		Action_Walk(playercomp);
		break;
	case VisPred::Game::EFSM::RUN:
		Action_Run(playercomp);
		break;
	case VisPred::Game::EFSM::CROUCH:
		Action_Crouch(playercomp);
		break;
	case VisPred::Game::EFSM::SLIDE:
		Action_Slide(playercomp,deltaTime);
		break;
	case VisPred::Game::EFSM::JUMP:
		Action_Jump(playercomp);
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
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Move_Rotation(playercomp, transfomcomp);
}

void PlayerSystem::Action_Slide(PlayerComponent& playercomp,float deltatime)
{
	///지면 기준에서 하기.
	///Input이 있을경우에는 그 방향으로 이동해야함.
	/// 콜라이더 오프셋 기능 추가.

	if (!playercomp.HasComponent<ControllerComponent>())
		return; 

	auto& controller = *playercomp.GetComponent<ControllerComponent>();
	auto& transcomp = *playercomp.GetComponent<TransformComponent>();
	playercomp.SlideProgress += deltatime;



	auto slidespeed = playercomp.RunSpeed * 1.5;
	controller.MaxSpeed = playercomp.RunSpeed * 1.5;
	controller.Acceleration = controller.MaxSpeed * 3;

	//Move_Walk(transcomp, playercomp, controllercomp);
	Move_Rotation(playercomp, transcomp);
	
}

void PlayerSystem::Action_Walk(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.WalkSpeed;
	Controller.Acceleration = Controller.MaxSpeed * 3;


	Move_Walk(transfomcomp, playercomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
}

void PlayerSystem::Action_Run(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.RunSpeed;
	Controller.Acceleration = Controller.MaxSpeed * 3;

	Move_Walk(transfomcomp, playercomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
}
void PlayerSystem::Action_Crouch(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.WalkSpeed / 2.f;
	Controller.Acceleration = Controller.MaxSpeed * 3;

	Move_Walk(transfomcomp, playercomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
}

void PlayerSystem::Action_Jump(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Move_Walk(transfomcomp, playercomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
	Move_Jump(transfomcomp, Controller);
}

void PlayerSystem::Action_Attack(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Move_Walk(transfomcomp, playercomp, Controller);
	Move_Rotation(playercomp, transfomcomp);
	switch (playercomp.ShootType)
	{
	case VisPred::Game::GunType::PISTOL:
		Shoot_Pistol(playercomp);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		Shoot_ShotGun(playercomp);
		break;
	case VisPred::Game::GunType::RIFLE:
		Shoot_Rifle(playercomp);
		break;
	default:
		break;
	}
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

void PlayerSystem::GunCooltime(PlayerComponent& playercomp)
{
}

#pragma endregion

#pragma region Move Logic

void PlayerSystem::Move_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp)
{							 
	controllercomp.InputDir.x = {};
	controllercomp.InputDir.z = {};
	if (INPUTKEY(KEYBOARDKEY::W))
		controllercomp.InputDir += transformcomp.FrontVector;
	if (INPUTKEY(KEYBOARDKEY::S))
		controllercomp.InputDir -= transformcomp.FrontVector;
	if (INPUTKEY(KEYBOARDKEY::A))
		controllercomp.InputDir -= transformcomp.RightVector;
	if (INPUTKEY(KEYBOARDKEY::D))
		controllercomp.InputDir += transformcomp.RightVector;

	SetSlideDir(playercomp, controllercomp);
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
	if (INPUTKEYDOWN(KEYBOARDKEY::SPACE)|| INPUTKEY(KEYBOARDKEY::SPACE))
		controllercomp.InputDir += transformcomp.UpVector;
}

void PlayerSystem::Move_Slide(PlayerComponent& playercomp)
{
}
void PlayerSystem::Initialize()
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		Start(playercomp.GetEntityID());
	}
}
void PlayerSystem::Start(uint32_t gameObjectId)
{
	if (GetSceneManager()->HasComponent<PlayerComponent>(gameObjectId))
	{
		auto playercomp = GetSceneManager()->GetComponent<PlayerComponent>(gameObjectId);
		playercomp->DefalutCameraPos;
		playercomp->SitCameraPos;
		auto cameraentity = GetSceneManager()->GetChildEntityByName(playercomp->GetEntityID(), "PlayerCamera");
		auto cameracomp = cameraentity->GetComponent<TransformComponent>();
		playercomp->DefalutCameraPos = cameracomp->Local_Location;

		auto& controllercomp = *playercomp->GetComponent<ControllerComponent>();
		// Current full height of the capsule (Total Height = 2 * (radius + height))
		float fullHeight = 2 * (controllercomp.CapsuleControllerinfo.radius + controllercomp.CapsuleControllerinfo.height);
		// New height after crouching (half of the full height)
		float SitHeight = ((fullHeight / 2) - 2 * controllercomp.CapsuleControllerinfo.radius) / 2;
		// Ensure the new height doesn't become negative
		if (SitHeight < 0.01f)  // You can set a reasonable minimum threshold
			SitHeight = 0.01f;
		float heightReduction = controllercomp.CapsuleControllerinfo.height - SitHeight;
		playercomp->SitHeight = SitHeight;
		playercomp->SitHeightDiff = heightReduction / 2;
		playercomp->SitCameraPos.y = playercomp->DefalutCameraPos.y- playercomp->SitHeightDiff;
	};

}
void PlayerSystem::Finish(uint32_t gameObjectId)
{
}
void PlayerSystem::Finalize()
{
}
void PlayerSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		GunCooltime(playercomp);
		UpdateCharDataToController(playercomp);
		CarmeraPosChange(playercomp, deltaTime);
	}
}
#pragma endregion
