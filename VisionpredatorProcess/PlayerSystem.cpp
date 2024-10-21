#include "pch.h"
#include "PlayerSystem.h"
#include <iostream>
#include "EngineStructs.h"
#include <random>
using namespace VisPred::Game;

PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

#pragma region Update
void PlayerSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		Gun_Cooltime(playercomp, deltaTime);
		CameraShake(playercomp, deltaTime);
		Calculate_FSM(playercomp);
		FSM_Sound_FSM(playercomp, deltaTime);
		FSM_Action_FSM(playercomp, deltaTime);
		PlayerAnimation(playercomp);
		Active_VPMode(playercomp);
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
void PlayerSystem::Active_VPMode(PlayerComponent& playercomp)
{
	static bool temp = false;
	if (INPUTKEYDOWN(KEYBOARDKEY::R))
	{
		temp = !temp;
		m_Graphics->SetVP(temp);
	}
}
#pragma region Camera Setting
void PlayerSystem::DownCamera(PlayerComponent& playercomp, float deltatime)
{
	auto camPoseEntity = playercomp.CameraPosEntity.lock(); /*GetSceneManager()->GetEntity(playercomp.CameraPosID);*/
	if (!camPoseEntity)
		return;
	auto& posTransComp = *camPoseEntity->GetComponent<TransformComponent>();
	///카메라와 앉은 위치의 pos 와의 거리가 0.01 보다 작을 때.
	if ((playercomp.DownCameraPos - posTransComp.Local_Location).Length() < 0.01f)
	{
		playercomp.DownCameraPos = posTransComp.Local_Location;
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
		Temp = VPMath::Vector3::Lerp(playercomp.DefalutCameraPos, playercomp.DownCameraPos, (playercomp.CamTransProgress / playercomp.CamTransDuration));
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
		Temp = VPMath::Vector3::Lerp(playercomp.DownCameraPos, playercomp.DefalutCameraPos, (playercomp.CamTransProgress / playercomp.CamTransDuration));
		posTransComp.SetLocalLocation(Temp);
	}
}
void PlayerSystem::CarmeraPosChange(PlayerComponent& playercomp, float deltatime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::CROUCH:
	{
		playercomp.DownCameraPos = playercomp.SitCameraPos;
		DownCamera(playercomp, deltatime);
	}
	break;
	case VisPred::Game::PlayerFSM::SLIDE:
	{
		playercomp.DownCameraPos = playercomp.SlideCameraPos;
		DownCamera(playercomp, deltatime);
	}
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
#pragma region Player Attack
void PlayerSystem::PlayerMeleeAttack(PlayerComponent& playercomp)
{
	auto& meleecomp = *playercomp.GetComponent<PlayerMeleeComponent>();
	if (!playercomp.IsVPMode)
		Melee_Default(playercomp);
	else
		Melee_VPMode(playercomp);
}

void PlayerSystem::Melee_Default(PlayerComponent& playercomp)
{
	auto& meleecomp = *playercomp.GetComponent<PlayerMeleeComponent>();
	auto& PlayerAni = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();

	///공격이 가능한 상태인가?

	if (PlayerAni.IsBlending || PlayerAni.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Sword))
		return;

	///어택모드 설정하기.
	int attackmode = static_cast<int>(meleecomp.AttackMode);
	attackmode++;

	if (attackmode >3)
		meleecomp.AttackMode = VisPred::Game::PlayerMelee::Sword_First;
	else
		meleecomp.AttackMode = static_cast<VisPred::Game::PlayerMelee>(attackmode);

	///meleecomp.AttackMode 에 따른 설정 및 애니메이션 셜정.
	auto SetAttackDetails = [&](VisPred::Game::PlayerAni aniIndex, float length, float damage, float angle)
		{
			ChangeAni_Index(PlayerAni.GetEntityID(), aniIndex, 0, 0, false, false);
			float duration = m_Graphics->GetDuration(PlayerAni.FBX, static_cast<int>(aniIndex));
			float speed = std::get<1>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			float transitionDuration = std::get<2>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			meleecomp.Length = length;
			meleecomp.Damage = damage;
			meleecomp.Angle = angle;
			meleecomp.Time = duration / speed + transitionDuration;
			meleecomp.IsLeft = true;
		};

	switch (meleecomp.AttackMode) 
	{
	case VisPred::Game::PlayerMelee::Sword_First:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack1_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, meleecomp.SwordAngle);
		break;
	case VisPred::Game::PlayerMelee::Sword_Third:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack2_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, meleecomp.SwordAngle);
		break;
	case VisPred::Game::PlayerMelee::Sword_Second:
	case VisPred::Game::PlayerMelee::Sword_Fourth:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack3_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, 1);
		break;
	default:
		return;
	}

	// Spawn the prefab
	auto& camerapostrans = *playercomp.CameraPosEntity.lock()->GetComponent<TransformComponent>();
	auto& entity = *GetSceneManager()->SpawnEditablePrefab(meleecomp.DefalutPrefab, camerapostrans.World_Location, camerapostrans.World_Quaternion, { 1, 1, 1 });

	if (!entity.HasComponent<TrunComponent>())
		return;

	auto turncomp = entity.GetComponent<TrunComponent>();
	turncomp->Angle = meleecomp.Angle;
	turncomp->Is_Left = meleecomp.IsLeft;
	turncomp->MoveTime = meleecomp.Time*0.6;
	turncomp->Is_X = true;
	// Process children for area attack
	if (entity.HasComponent<Children>())
	{
		auto childIDs = entity.GetComponent<Children>()->ChildrenID;
		for (auto childID : childIDs) 
		{
			auto childEntity = GetSceneManager()->GetEntity(childID);
			if (!childEntity || !childEntity->HasComponent<RigidBodyComponent>() || !childEntity->HasComponent<AreaAttackComponent>())
				continue;
			auto& transcomp = *childEntity->GetComponent<TransformComponent>();
			auto& areaattack = *childEntity->GetComponent<AreaAttackComponent>();
			areaattack.Damage = meleecomp.Damage;
			transcomp.SetLocalLocation({ 0, 0, (playercomp.Radius + (meleecomp.Length / 2)) });
			transcomp.SetLocalScale({ 0.01, 0.01, meleecomp.Length / 2 });
			break;
		}
	}

}
void PlayerSystem::Melee_VPMode(PlayerComponent& playercomp)
{
	auto& meleecomp = *playercomp.GetComponent<PlayerMeleeComponent>();
	auto& PlayerAni = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();

	///공격이 가능한 상태인가?

	if (PlayerAni.IsBlending || PlayerAni.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Sword))
		return;

	///어택모드 설정하기.
	int attackmode = static_cast<int>(meleecomp.AttackMode);
	attackmode++;

	if (attackmode > 3)
		meleecomp.AttackMode = VisPred::Game::PlayerMelee::Sword_First;
	else
		meleecomp.AttackMode = static_cast<VisPred::Game::PlayerMelee>(attackmode);

	///meleecomp.AttackMode 에 따른 설정 및 애니메이션 셜정.
	auto SetAttackDetails = [&](VisPred::Game::PlayerAni aniIndex, float length, float damage, float angle)
		{
			ChangeAni_Index(PlayerAni.GetEntityID(), aniIndex, 0, 0, false, false);
			float duration = m_Graphics->GetDuration(PlayerAni.FBX, static_cast<int>(aniIndex));
			float speed = std::get<1>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			float transitionDuration = std::get<2>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			meleecomp.Length = length;
			meleecomp.Damage = damage;
			meleecomp.Angle = angle;
			meleecomp.Time = duration / speed + transitionDuration;
			meleecomp.IsLeft = true;
		};

	switch (meleecomp.AttackMode)
	{
	case VisPred::Game::PlayerMelee::Sword_First:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack1_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, meleecomp.SwordAngle);
		break;
	case VisPred::Game::PlayerMelee::Sword_Third:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack2_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, meleecomp.SwordAngle);
		break;
	case VisPred::Game::PlayerMelee::Sword_Second:
	case VisPred::Game::PlayerMelee::Sword_Fourth:
		SetAttackDetails(VisPred::Game::PlayerAni::ToAttack3_Sword, meleecomp.SwordLength, meleecomp.SwordDamage, 1);
		break;
	default:
		return;
	}

	// Spawn the prefab
	auto& camerapostrans = *playercomp.CameraPosEntity.lock()->GetComponent<TransformComponent>();
	auto& entity = *GetSceneManager()->SpawnEditablePrefab(meleecomp.DefalutPrefab, camerapostrans.World_Location, camerapostrans.World_Quaternion, { 1, 1, 1 });

	if (!entity.HasComponent<TrunComponent>())
		return;

	auto turncomp = entity.GetComponent<TrunComponent>();
	turncomp->Angle = meleecomp.Angle;
	turncomp->Is_Left = meleecomp.IsLeft;
	turncomp->MoveTime = meleecomp.Time * 0.6;
	turncomp->Is_X = true;
	// Process children for area attack
	if (entity.HasComponent<Children>())
	{
		auto childIDs = entity.GetComponent<Children>()->ChildrenID;
		for (auto childID : childIDs)
		{
			auto childEntity = GetSceneManager()->GetEntity(childID);
			if (!childEntity || !childEntity->HasComponent<RigidBodyComponent>() || !childEntity->HasComponent<AreaAttackComponent>())
				continue;
			auto& transcomp = *childEntity->GetComponent<TransformComponent>();
			auto& areaattack = *childEntity->GetComponent<AreaAttackComponent>();
			areaattack.Damage = meleecomp.Damage;
			transcomp.SetLocalLocation({ 0, 0, (playercomp.Radius + (meleecomp.Length / 2)) });
			transcomp.SetLocalScale({ 0.01, 0.01, meleecomp.Length / 2 });
			break;
		}
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
	case VisPred::Game::PlayerFSM::SLIDE:
		Calculate_Slide(playercomp);
		break;

	case VisPred::Game::PlayerFSM::DIE:
		Calculate_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DESTROY:
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

	if (playercomp.IsVPMode)
	{
		if (INPUTKEYDOWNS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		{
			if (playercomp.HasComponent<ControllerComponent>())
			{
				auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
				SetSlideDir(playercomp, controllercomp);
			}
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::SLIDE;
		}
		///점프
		else if (INPUTKEY(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
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
			CrouchModeController(playercomp);
			if (playercomp.HasComponent<ControllerComponent>())
			{
				auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
				SetSlideDir(playercomp, controllercomp);
			}
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::SLIDE;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
		{
			CrouchModeController(playercomp);
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::CROUCH;
		}
		///점프
		else if (INPUTKEY(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;

	}

}
void PlayerSystem::Calculate_Die(PlayerComponent& playercomp)
{
}
void PlayerSystem::Calculate_Walk(PlayerComponent& playercomp)
{
	if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
	{
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
	}
	else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
	else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL) || INPUTKEY(KEYBOARDKEY::LCONTROL))
	{
		CrouchModeController(playercomp);
		playercomp.CurrentFSM = VisPred::Game::PlayerFSM::CROUCH;
	}
}
void PlayerSystem::Calculate_Run(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::LSHIFT))
		{
			if (playercomp.HasComponent<ControllerComponent>())
			{
				auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
				SetSlideDir(playercomp, controllercomp);
			}
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::SLIDE;
		}
	}
	else
	{


		if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		else if (INPUTKEYUP(KEYBOARDKEY::LSHIFT))
		{
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}
		else if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || playercomp.GetComponent<ControllerComponent>()->IsFall)
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::JUMP;
		else if (INPUTKEYDOWN(KEYBOARDKEY::LCONTROL))
		{
			CrouchModeController(playercomp);
			if (playercomp.HasComponent<ControllerComponent>())
			{
				auto& controllercomp = *playercomp.GetComponent<ControllerComponent>();
				SetSlideDir(playercomp, controllercomp);
			}
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::SLIDE;
		}
	}

}
void PlayerSystem::Calculate_Crouch(PlayerComponent& playercomp)
{
	if (INPUTKEYUP(KEYBOARDKEY::LCONTROL))
	{
		if (INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
		{
			DefalutModeController(playercomp);
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}
		else
		{
			DefalutModeController(playercomp);
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
		}
	}
}
void PlayerSystem::Calculate_Slide(PlayerComponent& playercomp)
{
	if (playercomp.SlideProgress >= playercomp.SlideDuration)
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
			DefalutModeController(playercomp);

			if (!INPUTKEYS(KEYBOARDKEY::W, KEYBOARDKEY::A, KEYBOARDKEY::S, KEYBOARDKEY::D))
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::IDLE;
			else if (INPUTKEYS(KEYBOARDKEY::LSHIFT))
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
			else
				playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
		}


	}
}
void PlayerSystem::Calculate_Jump(PlayerComponent& playercomp)
{
	if (playercomp.GetComponent<ControllerComponent>()->IsFall)
		return;

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
		else if (INPUTKEY(KEYBOARDKEY::LSHIFT))
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::RUN;
		else
			playercomp.CurrentFSM = VisPred::Game::PlayerFSM::WALK;
	}

}
void PlayerSystem::Calculate_Destroy(PlayerComponent& playercomp)
{

}
#pragma endregion
#pragma endregion
#pragma region FSM Action
#pragma region FSM Action Main
void PlayerSystem::FSM_Action_FSM(PlayerComponent& playercomp, float deltaTime)
{

	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		FSM_Action_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		FSM_Action_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		FSM_Action_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		FSM_Action_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::SLIDE:
		FSM_Action_Slide(playercomp, deltaTime);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		FSM_Action_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		FSM_Action_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DESTROY:
		FSM_Action_Destroy(playercomp);
		break;
	default:
		break;
	}
}
#pragma endregion
#pragma region FSM Action Logic

void PlayerSystem::FSM_Action_Idle(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.InputDir = {};
	Active_Interect(playercomp);
	Active_Rotation(playercomp, transfomcomp);
	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Slide(PlayerComponent& playercomp, float deltatime)
{
	///지면 기준에서 하기.
	///Input이 있을경우에는 그 방향으로 이동해야함.
	/// 콜라이더 오프셋 기능 추가.
	auto& transcomp = *playercomp.GetComponent<TransformComponent>();
	Active_Slide(playercomp, deltatime);
	Active_Rotation(playercomp, transcomp);
	Active_Attack(playercomp);

}

void PlayerSystem::FSM_Action_Walk(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.MaxSpeed = playercomp.WalkSpeed;
	Controller.Acceleration = Controller.MaxSpeed * 3;

	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Interect(playercomp);

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
	Active_Interect(playercomp);

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
	Active_Interect(playercomp);

	Active_Attack(playercomp);
}

void PlayerSystem::FSM_Action_Jump(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Interect(playercomp);

	Active_Jump(transfomcomp, Controller);
	Active_Attack(playercomp);
}



void PlayerSystem::FSM_Action_Die(PlayerComponent& playercomp)
{

}

void PlayerSystem::FSM_Action_Destroy(PlayerComponent& playercomp)
{

}

#pragma endregion
#pragma endregion
#pragma region FSM Sound
#pragma region FSM Sound Main


void PlayerSystem::FSM_Sound_FSM(PlayerComponent& playercomp, float deltaTime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		FSM_Sound_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		FSM_Sound_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		FSM_Sound_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		FSM_Sound_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::SLIDE:
		FSM_Sound_Slide(playercomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		FSM_Sound_Jump(playercomp);
		break;

	case VisPred::Game::PlayerFSM::DIE:
		FSM_Sound_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DESTROY:
		FSM_Sound_Destroy(playercomp);
		break;
	default:
		break;
	}
}
#pragma endregion
#pragma region FSM Sound Logic
void PlayerSystem::FSM_Sound_Idle(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Sit = false;
	playerSoundcomp.Played_Run1 = false;
}
void PlayerSystem::FSM_Sound_Walk(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Sit = false;
	playerSoundcomp.Played_Run1 = false;

	if (!m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID(), playerSoundcomp.WalkSoundKey1) && !m_SoundEngine->IsPlayingSound(playercomp.GetEntityID(), playerSoundcomp.WalkSoundKey2))
	{
		m_SoundEngine->Stop(playerSoundcomp.GetEntityID());
		if (!playerSoundcomp.Played_Walk1)
			m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.WalkSoundKey1, playerSoundcomp.Volume_Walk, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
		else
			m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.WalkSoundKey2, playerSoundcomp.Volume_Walk, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
		playerSoundcomp.Played_Walk1 = !playerSoundcomp.Played_Walk1;
	}
}
void PlayerSystem::FSM_Sound_Run(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();
	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Sit = false;

	if (!m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID(), playerSoundcomp.RunSoundKey1)
		&& !m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID(), playerSoundcomp.RunSoundKey2))
	{
		m_SoundEngine->Stop(playerSoundcomp.GetEntityID());
		if (!playerSoundcomp.Played_Run1)
			m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.RunSoundKey1, playerSoundcomp.Volume_Run, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
		else
			m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.RunSoundKey2, playerSoundcomp.Volume_Run, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
		playerSoundcomp.Played_Run1 = !playerSoundcomp.Played_Run1;
	}
}
void PlayerSystem::FSM_Sound_Crouch(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Run1 = false;
	if (playerSoundcomp.Played_Sit)
		return;
	if (m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID()))
		m_SoundEngine->Stop(playerSoundcomp.GetEntityID());

	m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.SitSoundKey, playerSoundcomp.Volume_Sit, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
	playerSoundcomp.Played_Sit = true;
}
void PlayerSystem::FSM_Sound_Slide(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Sit = false;
	playerSoundcomp.Played_Run1 = false;
	if (playerSoundcomp.Played_Slide)
		return;
	if (m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID()))
		m_SoundEngine->Stop(playerSoundcomp.GetEntityID());

	m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.SlideSoundkey, playerSoundcomp.Volume_Slide, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
	playerSoundcomp.Played_Slide = true;

}
void PlayerSystem::FSM_Sound_Jump(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();
	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Sit = false;
	playerSoundcomp.Played_Run1 = false;
	if (playerSoundcomp.Played_Jump)
		return;
	if (m_SoundEngine->IsPlayingSound(playerSoundcomp.GetEntityID()))
		m_SoundEngine->Stop(playerSoundcomp.GetEntityID());

	m_SoundEngine->Play(playerSoundcomp.GetEntityID(), playerSoundcomp.JumpSoundkey, playerSoundcomp.Volume_Jump, playerSoundcomp.GetComponent<TransformComponent>()->World_Location);
	playerSoundcomp.Played_Jump = true;
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
	/*playercomp.IsRotated =*/ posTransComp->SetLocalRotation(cameraRotation);
}
void PlayerSystem::Active_Jump(const TransformComponent& transformcomp, ControllerComponent& controllercomp)
{
	controllercomp.InputDir.y = {};
	if (INPUTKEYDOWN(KEYBOARDKEY::SPACE) || INPUTKEY(KEYBOARDKEY::SPACE))
		controllercomp.InputDir += transformcomp.UpVector;
}
void PlayerSystem::Active_Slide(PlayerComponent& playercomp, float deltatime)
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

		if (playercomp.HasGun)
		{
			playercomp.GetComponent<PlayerMeleeComponent>()->AttackMode = VisPred::Game::PlayerMelee::END;
			auto& guncomp = *GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
			if (guncomp.IsEmpty)
			{
				if (Gun_Throw(playercomp, guncomp))
					Throw_Setting(playercomp);
			}
			else if (playercomp.ReadyToShoot)
				if (Gun_Shoot(playercomp, guncomp))
					Gun_RecoilSetting(playercomp, guncomp);
		}
		else if (!playercomp.HasGun)
			PlayerMeleeAttack(playercomp);
	}
}
#pragma endregion

#pragma region Animation 
void PlayerSystem::ChangeAni_Index(uint32_t entityID, VisPred::Game::PlayerAni playeraniIndex, float Speed, float transition, bool loop, bool Immidiate)
{
	VisPred::Engine::AniBlendData temp{ entityID ,static_cast<int>(playeraniIndex), Speed ,transition, loop };

	std::any data = temp;
	if (Immidiate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}
void PlayerSystem::ChangeAni_Index(uint32_t entityID, VisPred::Game::VPAni index, float Speed, float transition, bool loop, bool Immidiate)
{
	VisPred::Engine::AniBlendData temp{ entityID ,static_cast<int>(index), Speed ,transition, loop };

	std::any data = temp;
	if (Immidiate)
		EventManager::GetInstance().ImmediateEvent("OnChangeAnimation", data);
	else
		EventManager::GetInstance().ScheduleEvent("OnChangeAnimation", data);
}
void PlayerSystem::PlayerAnimation(PlayerComponent& playercomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsFinished)
	{
	AnimationFinished(playercomp, anicomp);
	ReturnToIdle(anicomp);
	}

}
void PlayerSystem::AnimationFinished(PlayerComponent& playercomp, AnimationComponent& anicomp)
{

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
			m_PhysicsEngine->AddVelocity(playercomp.ThrowingGunEntityID, temp, 35);
			socketcomp.ConnectedEntityID = 0;
			playercomp.ThrowingGunEntityID = 0;
			socketcomp.GetComponent<MeshComponent>()->IsOverDraw = false;
			playercomp.LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = true;
		}

	}
	break;
	default:
		break;
	}
}
void PlayerSystem::ReturnToIdle(AnimationComponent& anicomp)
{
	using namespace VisPred::Game;

	uint32_t entityID = anicomp.GetEntityID();
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::PlayerAni::ToAttack_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack_Rifle:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 0, 0, true); break;
	case  (int)VisPred::Game::PlayerAni::ToAttack1_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack2_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToAttack3_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 0, 0, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_Rifle:		ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 0, 0, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToThrow_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle01_Sword, 0, 0, false);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Sword:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Pistol:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_Rifle:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::ToIdle01_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 0, 0, true); break;	
	case  (int)VisPred::Game::PlayerAni::Tohook_Sword:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Sword, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_Pistol:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Pistol, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_Rifle:		ChangeAni_Index(entityID, PlayerAni::ToIdle02_Rifle, 0, 0, true);	break;
	case  (int)VisPred::Game::PlayerAni::Tohook_ShotGun:	ChangeAni_Index(entityID, PlayerAni::ToIdle02_ShotGun, 0, 0, true); break;

	default:
		break;
	}
}
void PlayerSystem::ReturnToVPIdle(PlayerComponent& playercomp, AnimationComponent& anicomp)
{
	uint32_t entityID = anicomp.GetEntityID();
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::VPAni::ToVP_attack_L:
	case  (int)VisPred::Game::VPAni::ToVP_attack_R:
	{
		switch (playercomp.CurrentFSM)
		{
		case VisPred::Game::PlayerFSM::IDLE:	ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);	break;
		case VisPred::Game::PlayerFSM::RUN:		ChangeAni_Index(entityID, VPAni::ToVP_run, 0, 0, true);	break;
		case VisPred::Game::PlayerFSM::JUMP:	ChangeAni_Index(entityID, VPAni::ToVP_jump, 0, 0, true);	break;
		default:
			break;
		}
	}
	break;
	case  (int)VisPred::Game::VPAni::ToVP_jump:
	{
		ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);
	}
	break;
	}

}
#pragma endregion
#pragma region Interect Logic
void PlayerSystem::Active_Interect(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
		return;
	if (INPUTKEYDOWN(KEYBOARDKEY::F))
	{
		Grab_Gun(playercomp);
	}
}

void PlayerSystem::Grab_Gun(PlayerComponent& playercomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Sword)
		return;

	if (playercomp.HasGun)
	{
		Change_Gun(playercomp);
		return;
	}
	auto gunentity = GetSceneManager()->GetEntity(playercomp.SearchedItemID);
	if (!gunentity || !gunentity->HasComponent<GunComponent>())
		return;


	uint32_t handID = playercomp.HandEntity.lock()->GetEntityID();

	auto guncomp = gunentity->GetComponent<GunComponent>();
	auto soceketcomp = gunentity->GetComponent<SocketComponent>();
	soceketcomp->IsConnected = true;
	soceketcomp->ConnectedEntityID = handID;
	playercomp.HasGun = true;
	playercomp.GunEntityID = guncomp->GetEntityID();
	playercomp.ShootType = guncomp->Type;
	guncomp->GetComponent<MeshComponent>()->MaskColor = {};
	guncomp->GetComponent<MeshComponent>()->IsOverDraw = true;
	///TODO 사운드 로직 추가하기.
	playercomp.LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = false;

	switch (guncomp->Type)
	{
	case VisPred::Game::GunType::PISTOL:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_Pistol, 0, 0, false);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_ShotGun, 0, 0, false);
		break;
	case VisPred::Game::GunType::RIFLE:
		ChangeAni_Index(handID, VisPred::Game::PlayerAni::ToIdle01_Rifle, 0, 0, false);
		break;
	default:
		break;
	}
}

void PlayerSystem::Drop_Gun(PlayerComponent& playercomp)
{
	if (!playercomp.HasGun)
		return;
	auto gunentity = GetSceneManager()->GetEntity(playercomp.SearchedItemID);

	auto guncomp = gunentity->GetComponent<GunComponent>();
	auto soceketcomp = gunentity->GetComponent<SocketComponent>();
	soceketcomp->IsConnected = false;
	soceketcomp->ConnectedEntityID = 0;
	playercomp.HasGun = true;
	playercomp.GunEntityID = guncomp->GetEntityID();
	playercomp.ShootType = guncomp->Type;
	guncomp->GetComponent<MeshComponent>()->MaskColor = {};
	guncomp->GetComponent<MeshComponent>()->IsOverDraw = true;
	///TODO 사운드 로직 추가하기.
	playercomp.LongswordEntity.lock().get()->GetComponent<MeshComponent>()->IsVisible = false;
	switch (guncomp->Type)
	{
	case VisPred::Game::GunType::PISTOL:
	case VisPred::Game::GunType::SHOTGUN:
	case VisPred::Game::GunType::RIFLE:
	{
		ChangeAni_Index(playercomp.HandEntity.lock()->GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_Sword, 0, 0, false);
		break;
	}
	default:
		break;
	}




}
void PlayerSystem::Change_Gun(PlayerComponent& playercomp)
{


}

#pragma endregion
#pragma region Gun Logic
bool PlayerSystem::Gun_Shoot(PlayerComponent& playercomp, GunComponent& guncomp)
{
	
	auto& TransformComp =*playercomp.FirePosEntity.lock()->GetComponent<TransformComponent>();
	switch (playercomp.ShootType)
	{
	case VisPred::Game::GunType::PISTOL:
		return Shoot_Pistol(playercomp, guncomp, TransformComp);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		return Shoot_ShotGun(playercomp, guncomp, TransformComp);
		break;
	case VisPred::Game::GunType::RIFLE:
		return Shoot_Rifle(playercomp, guncomp, TransformComp);
		break;
	default:
		return false;
		break;
	}
}
bool PlayerSystem::Gun_Throw(PlayerComponent& playercomp, GunComponent& guncomp)
{
	switch (playercomp.ShootType)
	{
	case VisPred::Game::GunType::PISTOL:
		return Throw_Pistol(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::SHOTGUN:
		return Throw_ShotGun(playercomp, guncomp);
		break;
	case VisPred::Game::GunType::RIFLE:
		return Throw_Rifle(playercomp, guncomp);
		break;
	default:
		return false;
		break;
	}

}
void PlayerSystem::Gun_RecoilSetting(PlayerComponent& playercomp, GunComponent& guncomp)
{
	playercomp.IsGunRecoiling = true;
	// 랜덤한 Yaw 회전 범위 [-RecoilPos.x, RecoilPos.x]에서 값 생성
	float randfloat = Randomfloat(-guncomp.RecoilPos.x, guncomp.RecoilPos.x);
	// 카메라의 TransformComponent 가져오기
	auto cameratrans = playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
	// 초기 및 최종 반동 Quaternion 설정
	playercomp.GunRecoilStartQuat = cameratrans->Local_Quaternion;
	playercomp.GunRecoilEndQuat = playercomp.GunRecoilStartQuat;
	// 랜덤한 Yaw 회전 및 Pitch 반동 적용
	playercomp.GunRecoilEndQuat.RotateYaw(randfloat);
	playercomp.GunRecoilEndQuat.RotateToUp(guncomp.RecoilPos.y);
	// 반동 Quaternion에서 Euler 각도 추출
	VPMath::Vector3 euler = playercomp.GunRecoilEndQuat.ToEuler();
	// 최대 Pitch 및 Yaw 각도 정의 (라디안 단위)
	const float maxPitch = guncomp.RecoilMaxXY.y * (VPMath::XM_PI / 180.0f);
	const float maxYaw = guncomp.RecoilMaxXY.x * (VPMath::XM_PI / 180.0f);
	// Pitch와 Yaw 값을 최대값으로 제한
	euler.x = std::clamp(euler.x, -maxPitch, maxPitch);
	euler.y = std::clamp(euler.y, -maxYaw, maxYaw);
	// 제한된 Euler 각도에서 최종 반동 Quaternion 설정
	playercomp.GunRecoilEndQuat = VPMath::Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
}
#pragma region Gun Recoil
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

		// Extract yaw, pitch, roll from quaternion

		cameratrans->SetLocalQuaternion(tempquat);

		if (playercomp.GunprogressTime > gunComp->RecoilTime)
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
			float temppercent = EndRecoilPercent(playercomp.RecoilProgress, playercomp.RecoilReturnTime);
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
		auto& cameratrans = *playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
		playercomp.RecoilProgress += deltatime;
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
		VPMath::Quaternion tempquat{};
		double percent = RecoilPercent(playercomp.GunprogressTime, gunComp->RecoilTime, gunComp->RecoilPercent);
		if (!RecoilReturn(playercomp.GunprogressTime, gunComp->RecoilTime, gunComp->RecoilPercent))
			tempquat = tempquat.Slerp(playercomp.GunRecoilStartQuat, playercomp.GunRecoilEndQuat, percent);
		else
			tempquat = tempquat.Slerp({}, playercomp.GunRecoilEndQuat, percent);


		cameratrans.SetLocalQuaternion(tempquat);

		if (percent >= 1.0f)
		{
			VPMath::Quaternion tempquat{};
			playercomp.RecoilProgress = 0;
			playercomp.GunRecoilStartQuat = {};
			playercomp.GunRecoilEndQuat = {};
			playercomp.IsGunRecoiling = false;
			//playercomp.IsEndReocilReturn = false;
			cameratrans.SetLocalQuaternion({});
		}
	}
}
#pragma endregion
#pragma region Shoot Logic
bool PlayerSystem::Shoot_Common(PlayerComponent& playercomp, GunComponent& guncomp, VisPred::Game::PlayerAni idleAnimation, VisPred::Game::PlayerAni attackAnimation)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();

	// Check if the player is in the correct animation state to shoot
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(idleAnimation))
		return false;
	// Change animation to attack animation
	ChangeAni_Index(anicomp.GetEntityID(), attackAnimation, 0, 0, false);
	// Update gun and player state
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	guncomp.CurrentBullet -= 1;
	return true;
}
bool PlayerSystem::Shoot_Pistol(PlayerComponent& playercomp, GunComponent& guncomp, TransformComponent& firetrans)
{
	if (!Shoot_Common(playercomp, guncomp, PlayerAni::ToIdle02_Pistol, PlayerAni::ToAttack_Pistol))
		return false;
	auto temppos = firetrans.World_Location;
	auto temprotate = firetrans.World_Rotation;
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
bool PlayerSystem::Shoot_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp, TransformComponent& firetrans)
{
	if (!Shoot_Common(playercomp, guncomp, PlayerAni::ToIdle02_ShotGun, PlayerAni::ToAttack_ShotGun))
		return false;

	auto temppos = firetrans.World_Location;
	auto temprotate = firetrans.World_Rotation;
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
bool PlayerSystem::Shoot_Rifle(PlayerComponent& playercomp, GunComponent& guncomp, TransformComponent& firetrans)
{
	if (!Shoot_Common(playercomp, guncomp, PlayerAni::ToIdle02_Rifle, PlayerAni::ToAttack_Rifle))
		return false;

	auto temppos = firetrans.World_Location;
	auto temprotate = firetrans.World_Rotation;
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.BulletPrefab, temppos, temprotate);
	m_SceneManager.lock()->SpawnEditablePrefab(guncomp.GunSoundPrefab, temppos, temprotate);
	return true;
}
#pragma endregion
#pragma region Throw Logic
bool PlayerSystem::Throw_Setting(PlayerComponent& playercomp)
{
	playercomp.GunprogressTime = 0;
	playercomp.ReadyToShoot = false;
	playercomp.HasGun = false;
	playercomp.ThrowingGunEntityID = playercomp.GunEntityID;
	playercomp.GunEntityID = 0;
	playercomp.ShootType = VisPred::Game::GunType::NONE;
	return true;
}
bool PlayerSystem::Throw_Pistol(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Pistol))
		return false;

	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Pistol, 0, 0, false);
	return true;
}
bool PlayerSystem::Throw_ShotGun(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_ShotGun))
		return false;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_ShotGun, 0, 0, false);
	return true;

}
bool PlayerSystem::Throw_Rifle(PlayerComponent& playercomp, GunComponent& guncomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending || anicomp.curAni != static_cast<int>(VisPred::Game::PlayerAni::ToIdle02_Rifle))
		return false;
	ChangeAni_Index(anicomp.GetEntityID(), VisPred::Game::PlayerAni::ToThrow_Rifle, 0, 0, false);
	return true;
}
#pragma endregion
void PlayerSystem::Gun_Cooltime(PlayerComponent& playercomp, float deltatime)
{
	playercomp.GunprogressTime += deltatime;
	if (!playercomp.HasGun)
		playercomp.ReadyToShoot = false;
	else
	{
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);

		if (playercomp.GunprogressTime >= gunComp->CoolTime)
			playercomp.ReadyToShoot = true;
		else
			playercomp.ReadyToShoot = false;
	}
}
#pragma endregion 
#pragma region IStartable
void PlayerSystem::Initialize()
{
	COMPLOOP(PlayerComponent, playercomp)
		Start(playercomp.GetEntityID());
}
void PlayerSystem::Start(uint32_t gameObjectId)
{
	if (GetSceneManager()->HasComponent<PlayerComponent>(gameObjectId))
	{
		auto playercomp = GetSceneManager()->GetComponent<PlayerComponent>(gameObjectId);
		auto HandEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->HandName);
		auto VPHandEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->VPHandName);
		auto CameraEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->CameraName);
		auto FirePosEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->FirePosName);
		auto CameraPosEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->CameraPosName);
		auto LongswordEntity = GetSceneManager()->GetRelationEntityByName(gameObjectId, playercomp->LongswordName);
		playercomp->HP = playercomp->MaxHP;
		if (HandEntity)
			playercomp->HandEntity = HandEntity;			//playercomp->HandID = HandEntity->GetEntityID();
		else
			VP_ASSERT(false, "player의 손이 감지되지 않습니다.");
		if (VPHandEntity)
			playercomp->VPHandEntity = VPHandEntity;			//playercomp->HandID = HandEntity->GetEntityID();
		else
			VP_ASSERT(false, "player의 VPHand가 감지되지 않습니다.");

		if (LongswordEntity)
			playercomp->LongswordEntity = LongswordEntity;			//playercomp->HandID = HandEntity->GetEntityID();
		else
			VP_ASSERT(false, "LongswordEntity가 감지되지 않습니다.");


		playercomp->HandEntity.lock().get()->GetComponent<SkinningMeshComponent>()->IsOverDraw = true;
		playercomp->VPHandEntity.lock().get()->GetComponent<SkinningMeshComponent>()->IsOverDraw = true;
		playercomp->VPHandEntity.lock().get()->GetComponent<SkinningMeshComponent>()->IsVisible = false;
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
			playercomp->SlideCameraPos = playercomp->DefalutCameraPos;
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
			playercomp->Radius = controllercomp.CapsuleControllerinfo.radius;

			// 캡슐의 현재 전체 높이 (총 높이 = 2 * (반지름 + 높이))
			float fullHeight = 2 * (controllercomp.CapsuleControllerinfo.radius + controllercomp.CapsuleControllerinfo.height);
			// 앉을 때의 새로운 높이 (전체 높이의 절반)
			float SitHeight = ((fullHeight / 2) - 2 * controllercomp.CapsuleControllerinfo.radius) / 2;
			// 새로운 높이가 음수가 되지 않도록 보정
			if (SitHeight < 0.01f)
				SitHeight = 0.01f;

			float heightReduction = controllercomp.CapsuleControllerinfo.height - SitHeight;
			playercomp->SitHeight = SitHeight;
			playercomp->SitHeightDiff = heightReduction / 2;
			playercomp->SitCameraPos.y -= playercomp->SitHeightDiff;

			// 슬라이딩할 때의 새로운 높이 (전체 높이의 0.25)
			float SlideHeight = ((fullHeight * 0.25) - 2 * controllercomp.CapsuleControllerinfo.radius) / 2;

			// 새로운 슬라이드 높이가 음수가 되지 않도록 보정
			if (SlideHeight < 0.01f)
				SlideHeight = 0.01f;

			float slideHeightReduction = controllercomp.CapsuleControllerinfo.height - SlideHeight;
			playercomp->SlideHeight = SlideHeight;
			playercomp->SlideHeightDiff = slideHeightReduction / 2;
			playercomp->SlideCameraPos.y -= playercomp->SlideHeightDiff;
		}

		else
			VP_ASSERT(false, "player의 Controller가 감지되지 않습니다.");



	};
}
#pragma endregion
#pragma region player Functuions
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
	return (delta * delta) / (a * a);
}

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