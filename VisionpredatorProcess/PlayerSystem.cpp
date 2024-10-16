#include "pch.h"
#include "PlayerSystem.h"
#include <iostream>
#include "EngineStructs.h"
#include <random>
PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}
#pragma region player Functuions

float PlayerSystem::Randomfloat(float min, float max)
{
	// Create a random number generator
	std::random_device rd; // Seed generator
	std::mt19937 gen(rd()); // Mersenne Twister random engine

	// Create a distribution for floating-point numbers between min and max
	std::uniform_real_distribution<float> dist(min, max);

	// Return the random number
	return dist(gen);
}
double PlayerSystem::RecoilPercent(double x, double a, double percent)
{
	// Convert percent into a value between 0 and 1
	double b = percent / 100.0;

	// Ensure non-negative values
	if (x <= 0)
		return 0.0;

	if (x < a * b)
	{
		// First half: downward parabola
		double delta = x - a * b;
		return -(delta * delta) / (b * b * a * a) + 1;
	}
	else if (x >= a)
	{
		// Beyond full recoil
		return 0.0;
	}
	else
	{
		// Second half: upward parabola
		double delta = x - a;
		return (delta * delta) / ((1 - b) * (1 - b) * a * a);
	}
}

bool PlayerSystem::RecoilReturn(double x, double a, double percent)
{
	// Convert percent into a value between 0 and 1
	double b = percent / 100.0;

	// Ensure non-negative values

	if (x < a * b)
		return false;
	else
		return true;
}

double PlayerSystem::EndRecoilPercent(double x, double a)
{
	double delta = x - a;
	return (delta * delta) / ( a * a);
}


#pragma endregion
#pragma region Update

void PlayerSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		GunCooltime(playercomp, deltaTime);
		CameraShake(playercomp, deltaTime);
		//Gun_Recoiling(playercomp, deltaTime);
		Calculate_FSM(playercomp);
		FSM_Sound_FSM(playercomp, deltaTime);
		FSM_Action_FSM(playercomp, deltaTime);
		ToVPMode(playercomp);
		PlayerInterect(playercomp);
		PlayerAnimation(playercomp);
	}
}
void PlayerSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		SearchingInterectives(playercomp);
		UpdateCharDataToController(playercomp);
		CarmeraPosChange(playercomp, deltaTime);
	}
}
void PlayerSystem::PhysicsUpdate(float deltaTime)
{
}
void PlayerSystem::SoundUpdate(float deltaTime)
{
}
void PlayerSystem::PhysicsLateUpdate(float deltaTime)
{
}
#pragma endregion

#pragma region Searching interective
void PlayerSystem::SearchingInterectives(PlayerComponent& playercomp)
{
	/// Raycast 를 통해 오브젝트를 검출. 이전에 찾았던 오브젝트의 마스크를 0 0 0 처리해준다. 
	SearchInterective(playercomp);
	///찾아낸 물체가 Guncomp를 가지고있다면 
	SearchedGun(playercomp);
	//SearchedDoor(playercomp);
	//SearchedButton(playercomp);
}
void PlayerSystem::SearchInterective(PlayerComponent& playercomp)
{
	playercomp.PreSearchedItemID = playercomp.SearchedItemID;

	//auto posEntity = GetSceneManager()->GetEntity(playercomp.CameraID);
	auto cameraEntity = playercomp.CameraEntity.lock();
	if (!cameraEntity || !cameraEntity->HasComponent<CameraComponent>())
		return;
	auto cameratransform = cameraEntity->GetComponent<TransformComponent>();
	auto front = cameratransform->FrontVector;
	playercomp.SearchedItemID = m_PhysicsEngine->RaycastToHitActorFromLocation_Ignore(playercomp.GetEntityID(), cameratransform->World_Location, cameratransform->FrontVector, 20.f);

	if (playercomp.PreSearchedItemID != playercomp.SearchedItemID)
	{
		auto presearchedentity = GetSceneManager()->GetEntity(playercomp.PreSearchedItemID);
		if (presearchedentity && presearchedentity->HasComponent<MeshComponent>())
			presearchedentity->GetComponent<MeshComponent>()->MaskColor = { 0,0,0,0 };
	}
}
void PlayerSystem::SearchedGun(PlayerComponent& playercomp)
{
	auto gunentity = GetSceneManager()->GetEntity(playercomp.SearchedItemID);
	if (gunentity && gunentity->HasComponent<GunComponent>() && gunentity->GetEntityID() != playercomp.GunEntityID)
		gunentity->GetComponent<MeshComponent>()->MaskColor = { 255,0,0,255 };
}
#pragma endregion

void PlayerSystem::ToVPMode(PlayerComponent& playercomp)
{
	static bool temp = false;
	if (INPUTKEYDOWN(KEYBOARDKEY::R))
	{
		temp = !temp;
		m_Graphics->SetVP(temp);
	}
}
#pragma region Physics Setting

void PlayerSystem::UpdateCharDataToController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	//controllercomp.Acceleration= playercomp.Accel;
	//controllercomp.MaxSpeed = playercomp.WalkSpeed;
	controlcomp.JumpSpeed = playercomp.JumpForce;
	controlcomp.StaticFriction = playercomp.StaticFriction;
	controlcomp.DynamicFriction = playercomp.DynamicFriction;
	controlcomp.JumpXZAcceleration = controlcomp.Acceleration * playercomp.AirControlPercent / 100;
	controlcomp.GravityWeight = playercomp.GravityPower * 9.80665f;
}
void PlayerSystem::UpdateControllerSize(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.height, controlcomp.CapsuleControllerinfo.radius);
}
void PlayerSystem::CrouchModeController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	controlcomp.Contollerinfo.LocalOffset.y = -playercomp.SitHeightDiff;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, playercomp.SitHeight);
}
void PlayerSystem::DefalutModeController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	controlcomp.Contollerinfo.LocalOffset.y = 0;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, controlcomp.CapsuleControllerinfo.height);
}
#pragma endregion
#pragma region Camera Defalut Pos Setting
void PlayerSystem::DownCamera(PlayerComponent& playercomp, float deltatime)
{
	auto camPoseEntity = playercomp.CameraPosEntity.lock(); /*GetSceneManager()->GetEntity(playercomp.CameraPosID);*/
	if (!camPoseEntity)
		return;
	auto& posTransComp = *camPoseEntity->GetComponent<TransformComponent>();
	///카메라와 앉은 위치의 pos 와의 거리가 0.01 보다 작을 때.
	if ((playercomp.SitCameraPos - posTransComp.Local_Location).Length() < 0.01f)
	{
		playercomp.SitCameraPos = posTransComp.Local_Location;
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
		Temp = VPMath::Vector3::Lerp(playercomp.DefalutCameraPos, playercomp.SitCameraPos, (playercomp.CamTransProgress / playercomp.CamTransDuration));
		posTransComp.SetLocalLocation(Temp);
	}
}
void PlayerSystem::UpCamera(PlayerComponent& playercomp, float deltatime)
{
	auto camPoseEntity = playercomp.CameraPosEntity.lock(); /*GetSceneManager()->GetEntity(playercomp.CameraPosID);*/
	if (!camPoseEntity)
		return;
	auto& posTransComp = *camPoseEntity->GetComponent<TransformComponent>();
	///카메라와 앉은 위치의 pos 와의 거리가 0.01 보다 작을 때.
	if ((playercomp.DefalutCameraPos - posTransComp.Local_Location).Length() < 0.01f)
	{
		playercomp.DefalutCameraPos = posTransComp.Local_Location;
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
		posTransComp.SetLocalLocation(Temp);
	}

}
void PlayerSystem::CarmeraPosChange(PlayerComponent& playercomp, float deltatime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::CROUCH:
	case VisPred::Game::EFSM::SLIDE:
		DownCamera(playercomp, deltatime);
	break;
	default:
		UpCamera(playercomp, deltatime);
	break;
	}
}
void PlayerSystem::CameraShake(PlayerComponent& playercomp, float deltatime)
{
	switch (playercomp.RecoilMode)
	{
	case VisPred::Game::GunRecoilMode::ReturnToMiddle:
	Gun_RecoilingToMiddle(playercomp, deltatime);
		break;
	case VisPred::Game::GunRecoilMode::ReturnToEndAim:
	Gun_RecoilingToEnd(playercomp, deltatime);
		break;
	default:
		break;
	}
}
#pragma endregion 

#pragma region FSM_System
#pragma region FSM Calculate
#pragma region FSM Calculate Main
void PlayerSystem::Calculate_FSM(PlayerComponent& playercomp)
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
#pragma endregion
#pragma region FSM Calculate Logic
void PlayerSystem::Calculate_Idle(PlayerComponent& playercomp)
{
	///뛰기
	if ((INPUTKEYDOWN(KEYBOARDKEY::LSHIFT) || INPUTKEY(KEYBOARDKEY::LSHIFT))
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
		if (playercomp.HasComponent<ControllerComponent>())
		{
			auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
			SetSlideDir(playercomp, controllercomp);
		}
		playercomp.CurrentFSM = VisPred::Game::EFSM::SLIDE;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::EFSM::CROUCH;
	}
	///점프
	else if (INPUTKEY(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::EFSM::JUMP;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		if (playercomp.HasComponent<ControllerComponent>())
		{
			auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
			SetSlideDir(playercomp, controllercomp);
		}
		playercomp.CurrentFSM = VisPred::Game::EFSM::SLIDE;
	}

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
	if (playercomp.SlideProgress >= playercomp.SlideDuration)
	{
		playercomp.SlideProgress = 0;
		DefalutModeController(playercomp);

		if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
		else if (INPUTKEYS(KEYBOARDKEY::LSHIFT))
			playercomp.CurrentFSM = VisPred::Game::EFSM::RUN;
		else
			playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;

	}
}
void PlayerSystem::Calculate_Jump(PlayerComponent& playercomp)
{
	if (!playercomp.GetComponent<ControllerComponent>()->IsFall)
	{
		if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::EFSM::IDLE;
		else if (INPUTKEY(KEYBOARDKEY::LSHIFT))
			playercomp.CurrentFSM = VisPred::Game::EFSM::RUN;
		else
			playercomp.CurrentFSM = VisPred::Game::EFSM::WALK;
	}
}
void PlayerSystem::Calculate_Destroy(PlayerComponent& playercomp)
{
}
#pragma endregion
void PlayerSystem::SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp)
{
	if (playercomp.GetEntityID() != controllercomp.GetEntityID())
		return;
	if (controllercomp.InputDir.Length() < 0.01f)
		playercomp.SlideDir = playercomp.GetComponent<TransformComponent>()->FrontVector;
	else
		playercomp.SlideDir = controllercomp.InputDir;
}
#pragma endregion
#pragma region FSM Acting

#pragma region FSM Action
void PlayerSystem::FSM_Action_FSM(PlayerComponent& playercomp, float deltaTime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		FSM_Action_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::WALK:
		FSM_Action_Walk(playercomp);
		break;
	case VisPred::Game::EFSM::RUN:
		FSM_Action_Run(playercomp);
		break;
	case VisPred::Game::EFSM::CROUCH:
		FSM_Action_Crouch(playercomp);
		break;
	case VisPred::Game::EFSM::SLIDE:
		FSM_Action_Slide(playercomp, deltaTime);
		break;
	case VisPred::Game::EFSM::JUMP:
		FSM_Action_Jump(playercomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		FSM_Action_Attack(playercomp);
		break;
	case VisPred::Game::EFSM::DIE:
		FSM_Action_Die(playercomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
		FSM_Action_Destroy(playercomp);
		break;
	default:
		break;
	}
}

void PlayerSystem::FSM_Action_Idle(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.InputDir={};
	Active_Rotation(playercomp, transfomcomp);
	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Slide(PlayerComponent& playercomp, float deltatime)
{
	///지면 기준에서 하기.
	///Input이 있을경우에는 그 방향으로 이동해야함.
	/// 콜라이더 오프셋 기능 추가.
	auto& transcomp = *playercomp.GetComponent<TransformComponent>();
	Active_Slide(playercomp,deltatime);
	Active_Rotation(playercomp, transcomp);
}

void PlayerSystem::FSM_Action_Walk(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.WalkSpeed;
	Controller.Acceleration = Controller.MaxSpeed * 3;

	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Run(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.RunSpeed;
	Controller.Acceleration = Controller.MaxSpeed * 3;
	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Attack(playercomp);
}
void PlayerSystem::FSM_Action_Crouch(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.WalkSpeed / 2.f;
	Controller.Acceleration = Controller.MaxSpeed * 3;
	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Jump(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Jump(transfomcomp, Controller);
	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Attack(PlayerComponent& playercomp)
{

	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Rotation(playercomp, transfomcomp);

}

void PlayerSystem::FSM_Action_Die(PlayerComponent& playercomp)
{

}

void PlayerSystem::FSM_Action_Destroy(PlayerComponent& playercomp)
{

}

#pragma endregion
#pragma region FSM Sound
void PlayerSystem::FSM_Sound_FSM(PlayerComponent& playercomp, float deltaTime)
{	
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::EFSM::IDLE:
		FSM_Sound_Idle(playercomp);
		break;
	case VisPred::Game::EFSM::WALK:
		FSM_Sound_Walk(playercomp);
		break;
	case VisPred::Game::EFSM::RUN:
		FSM_Sound_Run(playercomp);
		break;
	case VisPred::Game::EFSM::CROUCH:
		FSM_Sound_Crouch(playercomp);
		break;
	case VisPred::Game::EFSM::SLIDE:
		FSM_Sound_Slide(playercomp);
		break;
	case VisPred::Game::EFSM::JUMP:
		FSM_Sound_Jump(playercomp);
		break;
	case VisPred::Game::EFSM::ATTACK:
		FSM_Sound_Attack(playercomp);
		break;
	case VisPred::Game::EFSM::DIE:
		FSM_Sound_Die(playercomp);
		break;
	case VisPred::Game::EFSM::DESTROY:
		FSM_Sound_Destroy(playercomp);
		break;
	default:
		break;
	}
}
void PlayerSystem::FSM_Sound_Idle(PlayerComponent& playercomp)
{
	playercomp.Played_Walk1 = false;
	playercomp.Played_Jump = false;
	playercomp.Played_Slide = false;
	playercomp.Played_Sit = false;
	playercomp.Played_Run1 = false;
}
void PlayerSystem::FSM_Sound_Walk(PlayerComponent& playercomp)
{
	playercomp.Played_Jump = false;
	playercomp.Played_Slide = false;
	playercomp.Played_Sit = false;
	playercomp.Played_Run1 = false;

	if (!m_SoundEngine->IsPlayingSound(playercomp.GetEntityID(), playercomp.WalkSoundKey1) && !m_SoundEngine->IsPlayingSound(playercomp.GetEntityID(), playercomp.WalkSoundKey2))
	{
		m_SoundEngine->Stop(playercomp.GetEntityID());
		if (!playercomp.Played_Walk1)
			m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.WalkSoundKey1, playercomp.Volume_Walk, playercomp.GetComponent<TransformComponent>()->World_Location);
		else
			m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.WalkSoundKey2, playercomp.Volume_Walk, playercomp.GetComponent<TransformComponent>()->World_Location);
		playercomp.Played_Walk1 = !playercomp.Played_Walk1;
	}
}
void PlayerSystem::FSM_Sound_Run(PlayerComponent& playercomp)
{
	playercomp.Played_Walk1 = false;
	playercomp.Played_Jump = false;
	playercomp.Played_Slide = false;
	playercomp.Played_Sit = false;

	if (!m_SoundEngine->IsPlayingSound(playercomp.GetEntityID(), playercomp.RunSoundKey1)
		&& !m_SoundEngine->IsPlayingSound(playercomp.GetEntityID(), playercomp.RunSoundKey2))
	{
		m_SoundEngine->Stop(playercomp.GetEntityID());
		if (!playercomp.Played_Run1)
			m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.RunSoundKey1, playercomp.Volume_Run, playercomp.GetComponent<TransformComponent>()->World_Location);
		else
			m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.RunSoundKey2, playercomp.Volume_Run, playercomp.GetComponent<TransformComponent>()->World_Location);
		playercomp.Played_Run1 = !playercomp.Played_Run1;
	}
}
void PlayerSystem::FSM_Sound_Crouch(PlayerComponent& playercomp)
{
	playercomp.Played_Walk1 = false;
	playercomp.Played_Jump = false;
	playercomp.Played_Slide = false;
	playercomp.Played_Run1 = false;
	if (playercomp.Played_Sit)
		return;
	if (m_SoundEngine->IsPlayingSound(playercomp.GetEntityID()))
		m_SoundEngine->Stop(playercomp.GetEntityID());

	m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.SitSoundKey, playercomp.Volume_Sit, playercomp.GetComponent<TransformComponent>()->World_Location);
	playercomp.Played_Sit = true;
}
void PlayerSystem::FSM_Sound_Slide(PlayerComponent& playercomp)
{
	playercomp.Played_Walk1 = false;
	playercomp.Played_Jump = false;
	playercomp.Played_Sit = false;
	playercomp.Played_Run1 = false;
	if (playercomp.Played_Slide)
		return;
	if (m_SoundEngine->IsPlayingSound(playercomp.GetEntityID()))
		m_SoundEngine->Stop(playercomp.GetEntityID());

	m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.SlideSoundkey, playercomp.Volume_Slide, playercomp.GetComponent<TransformComponent>()->World_Location);
	playercomp.Played_Slide = true;

}
void PlayerSystem::FSM_Sound_Jump(PlayerComponent& playercomp)
{
	playercomp.Played_Walk1 = false;
	playercomp.Played_Slide = false;
	playercomp.Played_Sit = false;
	playercomp.Played_Run1 = false;
	if (playercomp.Played_Jump)
		return;
	if (m_SoundEngine->IsPlayingSound(playercomp.GetEntityID()))
		m_SoundEngine->Stop(playercomp.GetEntityID());

	m_SoundEngine->Play(playercomp.GetEntityID(), playercomp.JumpSoundkey, playercomp.Volume_Jump, playercomp.GetComponent<TransformComponent>()->World_Location);
	playercomp.Played_Jump = true;
}
void PlayerSystem::FSM_Sound_Attack(PlayerComponent& playercomp)
{
}
void PlayerSystem::FSM_Sound_Die(PlayerComponent& playercomp)
{
}
void PlayerSystem::FSM_Sound_Destroy(PlayerComponent& playercomp)
{
}
#pragma endregion
#pragma endregion
#pragma endregion
#pragma region Active_logic
void PlayerSystem::Active_Walk(const TransformComponent& transformcomp, PlayerComponent& playercomp, ControllerComponent& controllercomp)
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
}
void PlayerSystem::Active_Rotation(PlayerComponent& playercomp, TransformComponent& transformcomp)
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

	auto posTransComp = playercomp.CameraPosEntity.lock()->GetComponent<TransformComponent>();/* GetSceneManager()->GetComponent<TransformComponent>(playercomp.CameraPosID);*/
	// 카메라 회전 업데이트
	float pitch = deltaCurposY * sensitivity;
	VPMath::Vector3 cameraRotation = posTransComp->Local_Rotation;
	cameraRotation.x += pitch;
	cameraRotation.x = std::clamp(cameraRotation.x, -89.9f, 89.9f);
	playercomp.IsRotated =posTransComp->SetLocalRotation(cameraRotation);
}
void PlayerSystem::Active_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp)
{
	controllercomp.InputDir.y = {};
	if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || INPUTKEY(KEYBOARDKEY::SPACE))
		controllercomp.InputDir += transformcomp.UpVector;
}
void PlayerSystem::Active_Slide(PlayerComponent& playercomp,float deltatime)
{
	auto& controller = *playercomp.GetComponent<ControllerComponent>();
	playercomp.SlideProgress += deltatime;
	auto slidespeed = playercomp.RunSpeed * 1.5;
	controller.MaxSpeed = playercomp.RunSpeed * 1.5;
	controller.Acceleration = controller.MaxSpeed * 3;
	controller.InputDir = playercomp.SlideDir;
}
void PlayerSystem::Active_Attack(PlayerComponent& playercomp)
{
	if (INPUTKEY(MOUSEKEY::LBUTTON))
	{
		if (playercomp.HasGun && playercomp.ReadyToShoot)
		{
			auto& guncomp = *GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
			if (!guncomp.IsEmpty)
			{
				if (Gun_Shoot(playercomp, guncomp))
				{
					playercomp.IsGunRecoiling = true;
					float randfloat = guncomp.RecoilPos.x;
					randfloat = Randomfloat(-randfloat, randfloat);
					auto cameratrans = playercomp.CameraEntity.lock()->GetComponent<TransformComponent>(); /*GetSceneManager()->GetComponent<TransformComponent>(playercomp.CameraID);*/
					playercomp.GunRecoilStartQuat = cameratrans->Local_Quaternion;
					playercomp.GunRecoilEndQuat = playercomp.GunRecoilStartQuat;
					playercomp.GunRecoilEndQuat.RotateYaw(randfloat);
					playercomp.GunRecoilEndQuat.RotateToUp(guncomp.RecoilPos.y);

					VPMath::Vector3 euler = playercomp.GunRecoilEndQuat.ToEuler();  // Use the ToEuler function to extract pitch

					const float maxPitch = guncomp.RecoilMaxXY.y * (VPMath::XM_PI / 180.0f);
					const float maxYaw = guncomp.RecoilMaxXY.x * (VPMath::XM_PI / 180.0f);
					if (euler.x > maxPitch)
						euler.x = maxPitch;
					else if (euler.x < -maxPitch)
						euler.x = -maxPitch;
					if (euler.y > maxYaw)
						euler.y = maxYaw;
					else if (euler.y < -maxYaw)
						euler.y = -maxYaw;

					playercomp.GunRecoilEndQuat = VPMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
				}
			}

			else
				Gun_Throw(playercomp, guncomp);
		}
		else if (!playercomp.HasGun)
		{



		}
	}
}
#pragma endregion
#pragma region Animation 
void PlayerSystem::ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni playeraniIndex, float Speed, bool loop, bool Immidiate)
{
	VisPred::Engine::AniBlendData temp{ entityID ,static_cast<int>(playeraniIndex), Speed , loop };

	std::any data = temp;
	if (Immidiate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}

void PlayerSystem::PlayerAnimation(PlayerComponent& playercomp)
{

	ThrowFinished(playercomp);
	ReturnToIdle(*playercomp.HandEntity.lock()->GetComponent<AnimationComponent>());
}

void PlayerSystem::ReturnToIdle(AnimationComponent& anicomp)
{
	using namespace VisPred::Game;
	if (anicomp.IsBlending || !anicomp.IsFinished)
		return;
	uint32_t entityID = anicomp.GetEntityID();
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::PlayerAni::ToAttack_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack_Rifle:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 1, true);break;
	case  (int)VisPred::Game::PlayerAni::ToAttack1_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack2_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack3_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 1, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_Rifle:		ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 1, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 1, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Rifle:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 1, true);break;	/*case  (int)VisPred::Game::PlayerAni::ToIdle02_Pistol:	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle02_Rifle:	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle02_ShotGun:	break;*/
	case  (int)VisPred::Game::PlayerAni::Tohook_Sword:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_Pistol:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_Rifle:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 1, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 1, true);break;	/*case  (int)VisPred::Game::PlayerAni::Tointeraction:		break;
	case  (int)VisPred::Game::PlayerAni::ToIdle02_Sword:	break;*/
	case  (int)VisPred::Game::PlayerAni::ToVP_attack_L:		ChangeAni_Index(entityID, PlayerAni::ToVP_Idle, 1, true);		break;
	case  (int)VisPred::Game::PlayerAni::ToVP_attack_R:		ChangeAni_Index(entityID, PlayerAni::ToVP_Idle, 1, true);		break;	/*case  (int)VisPred::Game::PlayerAni::ToVP_Idle:			break;
	case  (int)VisPred::Game::PlayerAni::ToVP_dash:			break;
	case  (int)VisPred::Game::PlayerAni::ToVP_jump:			break;
	case  (int)VisPred::Game::PlayerAni::ToVP_run:			break;*/
	case  (int)VisPred::Game::PlayerAni::ToVP_draw:			ChangeAni_Index(entityID, PlayerAni::ToVP_Idle, 1, true);		break;
	default:
		break;
	}
}
#pragma endregion

void PlayerSystem::ThrowFinished(PlayerComponent& playercomp)
{

	AnimationComponent& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();

	using namespace VisPred::Game;
	if (anicomp.IsBlending || !anicomp.IsFinished)
		return;
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::PlayerAni::ToThrow_Pistol:
	case  (int)VisPred::Game::PlayerAni::ToThrow_Rifle:
	case  (int)VisPred::Game::PlayerAni::ToThrow_ShotGun:
	{
		if (GetSceneManager()->GetEntity(playercomp.ThrowingGunEntityID))
		{
			auto& socketcomp = *GetSceneManager()->GetComponent<SocketComponent>(playercomp.ThrowingGunEntityID);
			VPMath::Vector3 temp = playercomp.FirePosEntity.lock()->GetComponent <TransformComponent>()->FrontVector;
			temp.RotateToUp(6);
			socketcomp.IsConnected = false;
			m_PhysicsEngine->AddVelocity(playercomp.ThrowingGunEntityID, temp,35);
			socketcomp.ConnectedEntityID = 0;
			playercomp.ThrowingGunEntityID = 0;
			socketcomp.GetComponent<MeshComponent>()->IsOverDraw = false;

		}
		
	}
	break;
	default:
		break;
	}
}

#pragma region Gun Logic
void PlayerSystem::PlayerInterect(PlayerComponent& playercomp)
{
	if (INPUTKEYDOWN(KEYBOARDKEY::F))
	{
		Grab_Gun(playercomp);

	}
}

void PlayerSystem::Gun_RecoilingToEnd(PlayerComponent& playercomp, float deltatime)
{
	if (playercomp.HasGun && playercomp.IsGunRecoiling)
	{
		playercomp.RecoilProgress += deltatime;
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
		auto cameratrans = playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
		VPMath::Quaternion tempquat{};
		double percent = RecoilPercent(playercomp.GunprogressTime, gunComp->RecoilTime, gunComp->RecoilPercent);

		tempquat = tempquat.Slerp(playercomp.GunRecoilStartQuat, playercomp.GunRecoilEndQuat, percent);
		VPMath::Quaternion tempquat1 = { tempquat.x,0,0,tempquat.w };
		VPMath::Quaternion tempquat2{};



		// Extract yaw, pitch, roll from quaternion

		cameratrans->SetLocalQuaternion(tempquat);

		if (playercomp.GunprogressTime> gunComp->RecoilTime)
		{
			VPMath::Quaternion tempquat{};
			playercomp.RecoilProgress = 0;
			playercomp.IsGunRecoiling = false;
			playercomp.IsEndReocilReturn = false;
			cameratrans->SetLocalQuaternion({});
		}
	}
	else if (playercomp.HasGun && !playercomp.IsGunRecoiling && !playercomp.IsEndReocilReturn)
	{
		playercomp.RecoilProgress += deltatime;
		VPMath::Quaternion tempquat{};
		auto cameratrans = playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
		if (playercomp.RecoilProgress <= playercomp.RecoilReturnTime)
		{
			double temppercent = EndRecoilPercent(playercomp.RecoilProgress, playercomp.RecoilReturnTime);
			tempquat = tempquat.Slerp({}, playercomp.GunRecoilStartQuat, temppercent);
			cameratrans->SetLocalQuaternion(tempquat);
		}
		else
		{
			cameratrans->SetLocalQuaternion({});
			playercomp.IsEndReocilReturn = true;
			playercomp.RecoilProgress = 0;
			playercomp.GunRecoilStartQuat = {};
			playercomp.GunRecoilEndQuat = {};
		}
	}

}
void PlayerSystem::Gun_RecoilingToMiddle(PlayerComponent& playercomp, float deltatime)
{
	if (playercomp.HasGun && playercomp.IsGunRecoiling)
	{
		playercomp.RecoilProgress += deltatime;
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
		auto cameratrans = playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
		VPMath::Quaternion tempquat{};
		double percent = RecoilPercent(playercomp.GunprogressTime, gunComp->RecoilTime, gunComp->RecoilPercent);
		if (!RecoilReturn(playercomp.GunprogressTime, gunComp->RecoilTime, gunComp->RecoilPercent))
			tempquat = tempquat.Slerp(playercomp.GunRecoilStartQuat, playercomp.GunRecoilEndQuat, percent);
		else
			tempquat = tempquat.Slerp({}, playercomp.GunRecoilEndQuat, percent);
		VPMath::Quaternion tempquat1 = { tempquat.x,0,0,tempquat.w };

		cameratrans->SetLocalQuaternion(tempquat);

		if (percent >= 1.0f)
		{
			VPMath::Quaternion tempquat{};
			playercomp.RecoilProgress = 0;
			playercomp.GunRecoilStartQuat = {};
			playercomp.GunRecoilEndQuat = {};
			playercomp.IsGunRecoiling = false;
			//playercomp.IsEndReocilReturn = false;
			cameratrans->SetLocalQuaternion({});
		}
	}
}

void PlayerSystem::Grab_Gun(PlayerComponent& playercomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Sword)
		return;

	if (playercomp.HasGun)
		return;
	auto gunentity = GetSceneManager()->GetEntity(playercomp.SearchedItemID);
	if (!gunentity || !gunentity->HasComponent<GunComponent>())
		return;

	auto socketentity = GetSceneManager()->GetEntitySocketEntity(gunentity->GetEntityID());
	if (!socketentity)
		return;
	uint32_t handID = playercomp.HandEntity.lock()->GetEntityID();

	auto guncomp = gunentity->GetComponent<GunComponent>();
	auto soceketcomp = socketentity->GetComponent<SocketComponent>();
	soceketcomp->IsConnected = true;
	soceketcomp->ConnectedEntityID = handID;
	playercomp.HasGun = true;
	playercomp.GunEntityID = guncomp->GetEntityID();
	playercomp.ShootType = guncomp->Type;
	guncomp->GetComponent<MeshComponent>()->MaskColor = {};
	guncomp->GetComponent<MeshComponent>()->IsOverDraw = true;
	///TODO 사운드 로직 추가하기.
	switch (guncomp->Type)
	{
	case VisPred::Game::GunType::PISTOL:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_Pistol, 4, false);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_ShotGun, 4, false);
		break;
	case VisPred::Game::GunType::RIFLE:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_Rifle, 4, false);
		break;
	default:
		break;
	}
}
bool PlayerSystem::Gun_Shoot(PlayerComponent& playercomp, GunComponent& guncomp)
{
	bool IsGunShoot = false;
	switch (playercomp.ShootType)
	{
	case VisPred::Game::GunType::PISTOL:
		return Shoot_Pistol(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		return Shoot_ShotGun(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::RIFLE:
		return Shoot_Rifle(playercomp, guncomp);
		break;
	default:
		break;
	}


}
void PlayerSystem::Gun_Throw(PlayerComponent& playercomp, GunComponent& guncomp)
{
	switch (playercomp.ShootType)
	{
	case VisPred::Game::GunType::PISTOL:
		Throw_Pistol(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		Throw_ShotGun(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::RIFLE:
		Throw_Rifle(playercomp, guncomp);
		break;
	default:
		break;
	}
}
#pragma region Gun System Logic
#pragma region Shoot Logic

bool PlayerSystem::Shoot_Pistol(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Pistol))
		return false;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	guncomp.CurrentBullet -= 1;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_Pistol, 4, false);

	auto posEntity = playercomp.FirePosEntity.lock();
	if (!posEntity)
		return false;
	auto tempTransform = posEntity->GetComponent<TransformComponent>();
	auto temppos = tempTransform->World_Location;
	auto temprotate = tempTransform->World_Rotation;
	m_SceneManager.lock()->SpawnPrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnPrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
bool PlayerSystem::Shoot_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_ShotGun))
		return false;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	guncomp.CurrentBullet -= 1;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_ShotGun, 4, false);

	auto posEntity = playercomp.FirePosEntity.lock(); 
	if (!posEntity)
		return false;
	auto tempTransform = posEntity->GetComponent<TransformComponent>();
	auto temppos = tempTransform->World_Location;
	auto temprotate = tempTransform->World_Rotation;
	m_SceneManager.lock()->SpawnPrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnPrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
bool PlayerSystem::Shoot_Rifle(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Rifle))
		return false;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	guncomp.CurrentBullet -= 1;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToAttack_Rifle, 10, false);
	auto posEntity = playercomp.FirePosEntity.lock();

	if (!posEntity)
		return false;
	auto tempTransform = posEntity->GetComponent<TransformComponent>();
	auto temppos = tempTransform->World_Location;
	auto temprotate = tempTransform->World_Rotation;
	m_SceneManager.lock()->SpawnPrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnPrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
#pragma endregion

#pragma region Throw Logic
void PlayerSystem::Throw_Pistol(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Pistol))
		return;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Pistol, 4, false);

	auto socketcomp = guncomp.GetComponent<SocketComponent>();
	//socketcomp->IsConnected = false;
	//socketcomp->ConnectedEntityID = 0;
	playercomp.HasGun = false;
	playercomp.ThrowingGunEntityID = playercomp.GunEntityID;
	playercomp.GunEntityID = 0;
	playercomp.ShootType = VisPred::Game::GunType::NONE;
}
void PlayerSystem::Throw_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_ShotGun))
		return;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_ShotGun, 4, false);

	auto socketcomp = guncomp.GetComponent<SocketComponent>();
	//socketcomp->IsConnected = false;
	//socketcomp->ConnectedEntityID = 0;
	playercomp.HasGun = false;
	playercomp.ThrowingGunEntityID = playercomp.GunEntityID;
	playercomp.GunEntityID = 0;
	playercomp.ShootType = VisPred::Game::GunType::NONE;
}
void PlayerSystem::Throw_Rifle(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();

	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Rifle))
		return;
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Rifle, 4, false);

	auto socketcomp = guncomp.GetComponent<SocketComponent>();
	//socketcomp->IsConnected = false;
	//socketcomp->ConnectedEntityID = 0;
	playercomp.HasGun = false;
	playercomp.ThrowingGunEntityID = playercomp.GunEntityID;
	playercomp.GunEntityID = 0;
	playercomp.ShootType = VisPred::Game::GunType::NONE;
}
#pragma endregion
void PlayerSystem::GunCooltime(PlayerComponent& playercomp, float deltatime)
{
	if (!playercomp.HasGun)
	{
		playercomp.GunprogressTime += deltatime;
		playercomp.ReadyToShoot = false;

	}
	else
	{
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
		if (playercomp.GunprogressTime >= gunComp->CoolTime)
		{
			playercomp.GunprogressTime += deltatime;
			playercomp.ReadyToShoot = true;
		}
		else
		{
			playercomp.GunprogressTime += deltatime;
			playercomp.ReadyToShoot = false;

		}
	}
}

#pragma endregion
#pragma endregion 
#pragma region IRenderable
void PlayerSystem::BeginRenderUpdate(float deltaTime)
{
}
void PlayerSystem::RenderUpdate(float deltaTime)
{
}
void PlayerSystem::LateRenderUpdate(float deltaTime)
{
}
void PlayerSystem::EditorRenderUpdate(float deltaTime)
{
}
#pragma endregion
#pragma region IStartable
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
		auto HandEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->HandName);
		auto CameraEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->CameraName);
		auto FirePosEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->FirePosName);
		auto CameraPosEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->CameraPosName);
		playercomp->HP= playercomp->MaxHP;
		if (HandEntity)
			playercomp->HandEntity = HandEntity;			//playercomp->HandID = HandEntity->GetEntityID();
		else
			VP_ASSERT(false, "player의 손이 감지되지 않습니다.");

		playercomp->HandEntity.lock().get()->GetComponent<SkinningMeshComponent>()->IsOverDraw = true;
		if (FirePosEntity)
			playercomp->FirePosEntity = FirePosEntity;
			//playercomp->FirePosEntityID = FirePosEntity->GetEntityID();
		else
			VP_ASSERT(false, "player의 FirePos 감지되지 않습니다.");

		if (CameraPosEntity)
		{
			playercomp->CameraPosEntity = CameraPosEntity;
			auto cameraposcomp = CameraPosEntity->GetComponent<TransformComponent>();
			playercomp->DefalutCameraPos = cameraposcomp->Local_Location;
			playercomp->SitCameraPos = playercomp->DefalutCameraPos;
		}
		else
			VP_ASSERT(false, "player의 Camerapos가 감지되지 않습니다.");

		if (CameraEntity)
			playercomp->CameraEntity = CameraEntity;
		else
			VP_ASSERT(false, "player의 카메라가 감지되지 않습니다.");

		if (playercomp->HasComponent<ControllerComponent>())
		{
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
			playercomp->SitCameraPos.y -= playercomp->SitHeightDiff;
		}
		else
			VP_ASSERT(false, "player의 Controller가 감지되지 않습니다.");



	};
}
void PlayerSystem::Finish(uint32_t gameObjectId)
{
}
void PlayerSystem::Finalize()
{
}
#pragma endregion
