#include "pch.h"
#include "VisPredComponents.h"
#include "EngineStructs.h"

#include "PlayerFSMSystem.h"
PlayerFSMSystem::PlayerFSMSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}

void PlayerFSMSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		if (!playercomp.CameraEntity.lock()
			|| !playercomp.CameraPosEntity.lock()
			|| !playercomp.FirePosEntity.lock()
			|| !playercomp.HandEntity.lock()
			|| !playercomp.VPHandEntity.lock()
			|| !playercomp.LongswordEntity.lock()
			)
			continue;
		Calculate_FSM(playercomp);
		Enter_FSM(playercomp);

	}

}

void PlayerFSMSystem::SoundUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		if (!playercomp.CameraEntity.lock()
			|| !playercomp.CameraPosEntity.lock()
			|| !playercomp.FirePosEntity.lock()
			|| !playercomp.HandEntity.lock()
			|| !playercomp.VPHandEntity.lock()
			|| !playercomp.LongswordEntity.lock()
			)
			continue;
		Enter_Sound_FSM(playercomp, deltaTime);

	}
}
void PlayerFSMSystem::LateUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		if (!playercomp.CameraEntity.lock()
			|| !playercomp.CameraPosEntity.lock()
			|| !playercomp.FirePosEntity.lock()
			|| !playercomp.HandEntity.lock()
			|| !playercomp.VPHandEntity.lock()
			|| !playercomp.LongswordEntity.lock()
			)
			continue;
		playercomp.PreFSM = playercomp.CurrentFSM;
	}
}

#pragma region FSM Calculate
#pragma region FSM Calculate Main

#pragma endregion
#pragma region FSM Calculate Logic
void PlayerFSMSystem::Calculate_FSM(PlayerComponent& playercomp)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Calculate_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Calculate_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Calculate_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Calculate_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Calculate_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Calculate_Slide(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Transformation:
		Calculate_Transformation(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		Calculate_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Calculate_Die_end(playercomp);
		break;
	default:
		break;
	}
}
void PlayerFSMSystem::Calculate_Idle(PlayerComponent& playercomp)
{

	if (playercomp.HP <= 0)
	{
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		return;
	}

	if (playercomp.IsVPMode)
	{
		if (INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		{
			auto& anicomp = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
			if (anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_L) || anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_R))
				return;
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Dash_Slide;


		}
		///점프
		else if (playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::R) && playercomp.ReadyToTransform)
		{
			///상태 저장
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Transformation;
		}

	}
	else
	{
		///뛰기
		if ((INPUTKEYDOWN(KEYBOARDKEY::LSHIFT) || INPUTKEY(KEYBOARDKEY::LSHIFT))
			&& INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		}
		///걷기
		else if (INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}
		///슬라이딩
		else if ((INPUTKEYDOWN(KEYBOARDKEY::LSHIFT) || INPUTKEY(KEYBOARDKEY::LSHIFT)) &&
			INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Dash_Slide;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::CROUCH;

		}
		///점프
		else if ( playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::R) && playercomp.ReadyToTransform)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Transformation;
	}
}
void PlayerFSMSystem::Calculate_Transformation(PlayerComponent& playercomp)
{
	if (playercomp.TransformationProgress >= playercomp.TransformationTime)
	{
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
	}
}
void PlayerFSMSystem::Calculate_Walk(PlayerComponent& playercomp)
{
	if (playercomp.HP <= 0)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
	else if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
	else if ( playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL) || INPUTKEY(KEYBOARDKEY::LCONTROL))
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::CROUCH;
	else if (INPUTKEYDOWN(KEYBOARDKEY::R) && playercomp.ReadyToTransform)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Transformation;
}
void PlayerFSMSystem::Calculate_Run(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		if (playercomp.HP <= 0)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		else if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else if (playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		{
			auto& anicomp = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
			if (anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_L) || anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_R))
				return;
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Dash_Slide;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::R) && playercomp.ReadyToTransform)
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Transformation;
		}
	}
	else
	{
		if (playercomp.HP <= 0)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		else if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else if (INPUTKEYUP(KEYBOARDKEY::LSHIFT))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		else if ( playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Dash_Slide;
		else if (INPUTKEYDOWN(KEYBOARDKEY::R) && playercomp.ReadyToTransform)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Transformation;
	}

}
void PlayerFSMSystem::Calculate_Crouch(PlayerComponent& playercomp)
{
	if (playercomp.HP <= 0)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
	else if (INPUTKEYUP(KEYBOARDKEY::LCONTROL))
	{
		if (INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}
		else
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		}
	}
}
void PlayerFSMSystem::Calculate_Slide(PlayerComponent& playercomp)
{
	if (playercomp.HP <= 0)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
	else if (playercomp.SlideProgress >= playercomp.SlideDuration)
	{
		playercomp.SlideProgress = 0;
		if (playercomp.IsVPMode)
		{
			if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
			else
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		}
		else
		{
			if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
			else if (INPUTKEYS(KEYBOARDKEY::LSHIFT))
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
			else
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}


	}
}
void PlayerFSMSystem::Calculate_Jump(PlayerComponent& playercomp)
{

	if (playercomp.IsVPMode)
	{
		if (playercomp.HP <= 0)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		{
			auto& anicomp = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
			if (anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_L) || anicomp.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_R))
				return;
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::Dash_Slide;
		}
	}

	if (playercomp.GetComponent<ControllerComponent>()->IsFall)
		return;

	if (playercomp.IsVPMode)
	{
		if (playercomp.HP <= 0)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		else if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
	}
	else
	{
		if (playercomp.HP <= 0)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE;
		else if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else if (INPUTKEY(KEYBOARDKEY::LSHIFT))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		else
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
	}

}
void PlayerFSMSystem::Calculate_Die(PlayerComponent& playercomp)
{
	if (playercomp.DieProgress > playercomp.DieTime)
	{
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::DIE_END;
		playercomp.DieProgress = playercomp.DieTime;
	}
}
void PlayerFSMSystem::Calculate_Die_end(PlayerComponent& playercomp)
{

}
void PlayerFSMSystem::Enter_Sound_FSM(PlayerComponent& playercomp, float deltaTime)
{
	if (playercomp.CurrentFSM == playercomp.PreFSM)
	{
		return;
	}
	auto& soundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Enter_Sound_Idle(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Enter_Sound_Walk(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Enter_Sound_Run(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Enter_Sound_Jump(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Enter_Sound_Crouch(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Enter_Sound_Dash_Slide(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::Transformation:
		Enter_Sound_Transformation(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		Enter_Sound_Die(soundcomp);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Enter_Sound_Die_end(soundcomp);
		break;
	default:
		break;
	}
}

void PlayerFSMSystem::Enter_Sound_Idle(PlayerSoundComponent& soundcomp)
{
	auto player = soundcomp.GetComponent<PlayerComponent>();
	if (player->PreFSM ==VisPred::Game::PlayerFSM::JUMP)
	{
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Run1, soundcomp.Volume_Run, true, false, {});
	}

}

void PlayerFSMSystem::Enter_Sound_Walk(PlayerSoundComponent& soundcomp)
{
	auto player = soundcomp.GetComponent<PlayerComponent>();
	if (player->PreFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Run1, soundcomp.Volume_Run, true, false, {});
	}
	soundcomp.Played_Walk1 = false;

}

void PlayerFSMSystem::Enter_Sound_Run(PlayerSoundComponent& soundcomp)
{
	auto player = soundcomp.GetComponent<PlayerComponent>();
	if (player->PreFSM == VisPred::Game::PlayerFSM::JUMP)
	{
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Run1, soundcomp.Volume_Run, true, false, {});
	}
	soundcomp.Played_Run1 = false;
}

void PlayerFSMSystem::Enter_Sound_Crouch(PlayerSoundComponent& soundcomp)
{
	auto& transform = *soundcomp.GetComponent<TransformComponent>();
	GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Sit, soundcomp.Volume_Sit, true, false, transform.World_Location);
}

void PlayerFSMSystem::Enter_Sound_Dash_Slide(PlayerSoundComponent& soundcomp)
{
	auto& transform = *soundcomp.GetComponent<TransformComponent>();
	if (soundcomp.HasComponent<PlayerComponent>()&&soundcomp.GetComponent<PlayerComponent>()->IsVPMode)
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Dash, soundcomp.Volume_Dash, true, false, transform.World_Location);
	else
	GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Slide, soundcomp.Volume_Slide, true, false, transform.World_Location);
}

void PlayerFSMSystem::Enter_Sound_Transformation(PlayerSoundComponent& soundcomp)
{
	auto& transform = *soundcomp.GetComponent<TransformComponent>();
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Transformation, soundcomp.Volume_Transformation, true, false, transform.World_Location);
}

void PlayerFSMSystem::Enter_Sound_Jump(PlayerSoundComponent& soundcomp)
{
	if (!INPUTKEY(KEYBOARDKEY::SPACE))
		return;
	auto& transform = *soundcomp.GetComponent<TransformComponent>();
	GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Jump, soundcomp.Volume_Jump, true, false, transform.World_Location);
}

void PlayerFSMSystem::Enter_Sound_Die(PlayerSoundComponent& soundcomp)
{
	auto& transform = *soundcomp.GetComponent<TransformComponent>();
		GetSceneManager()->SpawnSoundEntity(soundcomp.SoundKey_Death, soundcomp.Volume_Death, true, false, transform.World_Location);
}

void PlayerFSMSystem::Enter_Sound_Destroy(PlayerSoundComponent& soundcomp)
{

}

void PlayerFSMSystem::Enter_Sound_Die_end(PlayerSoundComponent& soundcomp)
{
	//GetSceneManager()->SpawnEditablePrefab("asdsad", {}, VPMath::Vector3{});
}

void PlayerFSMSystem::ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni index, float Speed, float transition, bool loop, bool Immidiate)
{
	VisPred::Engine::AniBlendData temp{ entityID ,static_cast<int>(index), Speed ,transition, loop };

	std::any data = temp;
	if (Immidiate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}

void PlayerFSMSystem::ChangeAni_Index(uint32_t entityID, VisPred::Game::VPAni index, float Speed, float transition, bool loop, bool Immidiate)
{
	VisPred::Engine::AniBlendData temp{ entityID ,static_cast<int>(index), Speed ,transition, loop };

	std::any data = temp;
	if (Immidiate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}

void PlayerFSMSystem::Enter_FSM(PlayerComponent& playercomp)
{
	if (playercomp.CurrentFSM == playercomp.PreFSM)
	{
		return;
	}

	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Enter_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Enter_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Enter_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Enter_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Enter_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Enter_Dash_Slide(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Transformation:
		Enter_Transformation(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		Enter_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Enter_Die_end(playercomp);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region FSM Enter Logic
void PlayerFSMSystem::Enter_Idle(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		auto anicomp = playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
		auto tempindex = static_cast<VisPred::Game::VPAni>(anicomp->curAni);
		if (tempindex == VisPred::Game::VPAni::ToVP_attack_L 
			|| tempindex == VisPred::Game::VPAni::ToVP_attack_R
			||tempindex == VisPred::Game::VPAni::ToVP_draw)
			return;
		ChangeAni_Index(playercomp.VPHandEntity.lock()->GetEntityID(), VisPred::Game::VPAni::ToVP_Idle, 0, 0, true);
	}
	else if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
		DefalutModeController(playercomp);

}
void PlayerFSMSystem::Enter_Walk(PlayerComponent & playercomp)
{
	if (!playercomp.IsVPMode)
		if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
			DefalutModeController(playercomp);
}
void PlayerFSMSystem::Enter_Run(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		auto anicomp = playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
		auto tempindex = static_cast<VisPred::Game::VPAni>(anicomp->curAni);
		if (tempindex == VisPred::Game::VPAni::ToVP_attack_L || tempindex == VisPred::Game::VPAni::ToVP_attack_R || tempindex == VisPred::Game::VPAni::ToVP_draw)
			return;
		ChangeAni_Index(playercomp.VPHandEntity.lock()->GetEntityID(), VisPred::Game::VPAni::ToVP_run,0,0,true);
	}
	else
	{
		if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
			DefalutModeController(playercomp);
	}

}
void PlayerFSMSystem::Enter_Jump(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		auto anicomp = playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
		auto tempindex = static_cast<VisPred::Game::VPAni>(anicomp->curAni);
		if (tempindex == VisPred::Game::VPAni::ToVP_attack_L || tempindex == VisPred::Game::VPAni::ToVP_attack_R)
			return;
		ChangeAni_Index(playercomp.VPHandEntity.lock()->GetEntityID(), VisPred::Game::VPAni::ToVP_Idle, 0, 0, true);
	}
}
void PlayerFSMSystem::Enter_Crouch(PlayerComponent& playercomp)
{
	CrouchModeController(playercomp);
}
void PlayerFSMSystem::Enter_Dash_Slide(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		ChangeAni_Index(playercomp.VPHandEntity.lock()->GetEntityID(), VisPred::Game::VPAni::ToVP_dash, 0, 0, true);
		playercomp.SlideDir = playercomp.GetComponent<TransformComponent>()->FrontVector;
		auto& melee = *playercomp.GetComponent<PlayerMeleeComponent>();
		auto& transform = *playercomp.GetComponent<TransformComponent>();
		auto Dastprefab = GetSceneManager()->SpawnEditablePrefab(melee.DashPrefab, transform.World_Location, { 0,0,0 }, { 0,0,0 });
		Dastprefab->GetComponent<RigidBodyComponent>()->CapsuleInfo.Radius = playercomp.Radius * 2.5f;
		Dastprefab->GetComponent<LifeTimeComponent>()->LifeTime = playercomp.SlideDuration;
		Dastprefab->GetComponent<AreaAttackComponent>()->IdentityAttach = playercomp.GetComponent<IdentityComponent>()->UUID;

	}
	else
	{
		SlideModeController(playercomp);
		SetSlideDir(playercomp, *playercomp.GetComponent<ControllerComponent>());
	}
}
void PlayerFSMSystem::Enter_Transformation(PlayerComponent& playercomp)
{
	Drop_Gun(playercomp);
	///변신 조절
	playercomp.ReadyToTransform = false;
	playercomp.IsTransformationing = true;
	playercomp.TransformationProgress = 0;
	playercomp.VPGageProgress = 0;
	playercomp.IsVPMode = !playercomp.IsVPMode;
	/// 무적 조절
	playercomp.MaxNonDamageTime = playercomp.TransformationTime + playercomp.NonDamageTime;
	playercomp.NonDamageProgress = 0;
	playercomp.NonDamageMode = true;
}
void PlayerFSMSystem::Enter_Die(PlayerComponent& playercomp)
{
}
void PlayerFSMSystem::Enter_Die_end(PlayerComponent& playercomp)
{
	GetSceneManager()->ChangeScene("../Data/Scene/Title.scene");
}

void PlayerFSMSystem::SetSlideDir(PlayerComponent& playercomp, ControllerComponent& controllercomp)
{
	if (playercomp.GetEntityID() != controllercomp.GetEntityID())
		return;
	if (controllercomp.InputDir.Length() < 0.01f)
		playercomp.SlideDir = playercomp.GetComponent<TransformComponent>()->FrontVector;
	else
		playercomp.SlideDir = controllercomp.InputDir;
}

void PlayerFSMSystem::Drop_Gun(PlayerComponent& playercomp)
{
	std::any data = std::ref(playercomp);
	EventManager::GetInstance().ImmediateEvent("OnDrop_Gun", data);
}

void PlayerFSMSystem::SlideModeController(PlayerComponent& playercomp)
{
	std::any data = std::ref(playercomp);
	EventManager::GetInstance().ImmediateEvent("OnSlideModeController",data);

}

void PlayerFSMSystem::CrouchModeController(PlayerComponent& playercomp)
{
	std::any data = std::ref(playercomp);
	EventManager::GetInstance().ImmediateEvent("OnCrouchModeController", data);
}

void PlayerFSMSystem::DefalutModeController(PlayerComponent& playercomp)
{
	std::any data = std::ref(playercomp);
	EventManager::GetInstance().ImmediateEvent("OnDefalutModeController", data);
}

#pragma endregion