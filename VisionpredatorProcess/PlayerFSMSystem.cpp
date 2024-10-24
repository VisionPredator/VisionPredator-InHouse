#include "pch.h"
#include "PlayerFSMSystem.h"
#include "VisPredComponents.h"
PlayerFSMSystem::PlayerFSMSystem(std::shared_ptr<SceneManager> sceneManager):System(sceneManager)
{
}

void PlayerFSMSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		Calculate_FSM(playercomp);
		Enter_Sound_FSM(playercomp, deltaTime);
	}

}

void PlayerFSMSystem::SoundUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{}
}

void PlayerFSMSystem::LateUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
		playercomp.PreFSM = playercomp.CurrentFSM;
}

#pragma region FSM Calculate
#pragma region FSM Calculate Main
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
#pragma endregion
#pragma region FSM Calculate Logic
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
		else if (INPUTKEY(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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
		else if (INPUTKEY(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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
		else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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
		else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
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

	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Enter_Sound_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Enter_Sound_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Enter_Sound_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Enter_Sound_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Enter_Sound_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Enter_Sound_Dash_Slide(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Transformation:
		Enter_Sound_Transformation(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		Enter_Sound_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Enter_Sound_Die_end(playercomp);
		break;
	default:
		break;
	}
}

void PlayerFSMSystem::Enter_Sound_Idle(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Walk(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Run(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Crouch(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Dash_Slide(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Transformation(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Jump(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Die(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Destroy(PlayerComponent& playercomp)
{
}

void PlayerFSMSystem::Enter_Sound_Die_end(PlayerComponent& playercomp)
{
}
