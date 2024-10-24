#include "pch.h"
#include "PlayerSystem.h"
#include <iostream>
#include "EngineStructs.h"
#include <random>
using namespace VisPred::Game;

PlayerSystem::PlayerSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{

}

bool PlayerSystem::ChangeArm(PlayerComponent& playercomp, bool IsVPmode)
{
	if (!playercomp.IsArmChanged)
	{
		if (IsVPmode)
		{
			playercomp.HandEntity.lock()->GetComponent<SkinningMeshComponent>()->IsVisible = false;
			playercomp.HandEntity.lock()->GetComponent<AnimationComponent>()->isPlay = false;
			playercomp.LongswordEntity.lock()->GetComponent<MeshComponent>()->IsVisible = false;

			playercomp.VPHandEntity.lock()->GetComponent<SkinningMeshComponent>()->IsVisible = true;
			playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>()->isPlay = true;
			ChangeAni_Index(playercomp.VPHandEntity.lock()->GetEntityID(), VisPred::Game::VPAni::ToVP_draw, 0, 0, false);
			m_Graphics->SetVP(playercomp.IsVPMode);
		}
		else
		{
			playercomp.HandEntity.lock()->GetComponent<SkinningMeshComponent>()->IsVisible = true;
			playercomp.HandEntity.lock()->GetComponent<AnimationComponent>()->isPlay = true;
			playercomp.LongswordEntity.lock()->GetComponent<MeshComponent>()->IsVisible = true;

			playercomp.VPHandEntity.lock()->GetComponent<SkinningMeshComponent>()->IsVisible = false;
			playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>()->isPlay = false;
			ChangeAni_Index(playercomp.HandEntity.lock()->GetEntityID(), VisPred::Game::PlayerAni::ToIdle01_Sword, 0, 0, false);
			m_Graphics->SetVP(playercomp.IsVPMode);

		}
		playercomp.IsArmChanged = true;
		return true;
	}
	return false;
}
void PlayerSystem::VPMode_Cooltime(PlayerComponent& playercomp, float deltatime)
{

	if (!playercomp.IsTransformationing)
	{
		if (playercomp.VPGageProgress < playercomp.VPGageCoolTime)
		{
			playercomp.VPGageProgress += deltatime;
			playercomp.ReadyToTransform = false;

		}
		else
		{
			playercomp.VPGageProgress = playercomp.VPGageCoolTime;
			playercomp.ReadyToTransform = true;
		}
	}
	else
	{
		playercomp.ReadyToTransform = false;


	}

}
void PlayerSystem::Transformation_Time(PlayerComponent& playercomp, float deltatime)
{
	if (!playercomp.IsTransformationing)
		return;
	if (playercomp.TransformationProgress > playercomp.TransformationTime)
	{
		playercomp.TransformationProgress = playercomp.TransformationTime;
		playercomp.IsTransformationing = false;
	}
	else
		playercomp.TransformationProgress += deltatime;
}
void PlayerSystem::NonDamage_Time(PlayerComponent& playercomp, float deltatime)
{
	if (playercomp.NonDamageMode)
	{
		if (playercomp.NonDamageProgress > playercomp.MaxNonDamageTime)
			playercomp.NonDamageMode = false;
		else
			playercomp.NonDamageProgress += deltatime;
	}
}
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
#pragma region Update
void PlayerSystem::Update(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
	{
		Transformation_Time(playercomp, deltaTime);
		Gun_Cooltime(playercomp, deltaTime);
		NonDamage_Time(playercomp, deltaTime);
		VPMode_Cooltime(playercomp, deltaTime);
		CameraShake(playercomp, deltaTime);
		Enter_FSM(playercomp);
		Action_FSM(playercomp, deltaTime);
		PlayerAnimation(playercomp);
		//Active_VPMode(playercomp);
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
void PlayerSystem::SoundUpdate(float deltaTime)
{
	COMPLOOP(PlayerComponent, playercomp)
		Sound_FSM(playercomp, deltaTime);
}


#pragma endregion

#pragma region Searching interective
void PlayerSystem::SearchingInterectives(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode || playercomp.IsTransformationing)
	{
		playercomp.SearchedItemID = 0;
		return;
	}
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
	playercomp.SearchedItemID = m_PhysicsEngine->RaycastToHitActorFromLocation_Ignore(playercomp.GetEntityID(), cameratransform->World_Location, cameratransform->FrontVector, playercomp.SearchDistance).EntityID;

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
//void PlayerSystem::Active_VPMode(PlayerComponent& playercomp)
//{
//	if (playercomp.ReadyToTransform)
//	{
//		if (INPUTKEYDOWN(KEYBOARDKEY::R)&& playercomp.ReadyToTransform)
//		{
//			playercomp.ReadyToTransform = false;
//		}
//	}
//}
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
	case VisPred::Game::PlayerFSM::Dash_Slide:
	{
		if (playercomp.IsVPMode)
			UpCamera(playercomp, deltatime);
		else
		{
			playercomp.DownCameraPos = playercomp.SlideCameraPos;
			DownCamera(playercomp, deltatime);
		}

	}
	break;
	default:
		UpCamera(playercomp, deltatime);
		break;
	}
}
void PlayerSystem::CameraShake(PlayerComponent& playercomp, float deltatime)
{
	Gun_RecoilingToMiddle(playercomp, deltatime);

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
	playercomp.IsAttacking = true;

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
			float duration = static_cast<float>(m_Graphics->GetDuration(PlayerAni.FBX, static_cast<int>(aniIndex)));
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
	turncomp->MoveTime = meleecomp.Time * 0.6f;
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
			transcomp.SetLocalLocation({ 0.f, 0.f, (playercomp.Radius + (meleecomp.Length / 2)) });
			transcomp.SetLocalScale({ 0.01f, 0.01f, meleecomp.Length / 2 });
			break;
		}
	}
}
void PlayerSystem::Melee_VPMode(PlayerComponent& playercomp)
{
	auto& meleecomp = *playercomp.GetComponent<PlayerMeleeComponent>();
	auto& PlayerAni = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();

	///공격이 가능한 상태인가?

	if (PlayerAni.IsBlending)
		return;
	if (PlayerAni.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_dash))
		return;
	if (!((PlayerAni.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_L) && PlayerAni.IsFinished) ||
		(PlayerAni.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_attack_R) && PlayerAni.IsFinished) ||
		PlayerAni.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_Idle) ||
		PlayerAni.curAni == static_cast<int>(VisPred::Game::VPAni::ToVP_run)))
		return;


	///어택모드 설정하기.
	int attackmode = static_cast<int>(meleecomp.AttackMode);
	attackmode++;

	if (attackmode >= 4 && attackmode <= 5)
		meleecomp.AttackMode = static_cast<VisPred::Game::PlayerMelee>(attackmode);
	else
		meleecomp.AttackMode = VisPred::Game::PlayerMelee::VP_Left;

	///meleecomp.AttackMode 에 따른 설정 및 애니메이션 셜정.
	auto SetAttackDetails = [&](VisPred::Game::VPAni aniIndex, float length, float damage, float angle)
		{
			ChangeAni_Index(PlayerAni.GetEntityID(), aniIndex, 0, 0, false, false);
			float duration = static_cast<float>(m_Graphics->GetDuration(PlayerAni.FBX, static_cast<int>(aniIndex)));
			float speed = std::get<1>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			float transitionDuration = std::get<2>(PlayerAni.AnimationSpeed_Transition[static_cast<int>(aniIndex)]);
			meleecomp.Length = length;
			meleecomp.Damage = damage;
			meleecomp.Angle = angle;
			meleecomp.Time = duration / speed + transitionDuration;
			if (aniIndex == VisPred::Game::VPAni::ToVP_attack_L)
				meleecomp.IsLeft = true;
			else
				meleecomp.IsLeft = false;
		};

	switch (meleecomp.AttackMode)
	{
	case VisPred::Game::PlayerMelee::VP_Left:
		SetAttackDetails(VisPred::Game::VPAni::ToVP_attack_L, meleecomp.VPLength, meleecomp.VPDamage, meleecomp.SwordAngle);
		break;
	case VisPred::Game::PlayerMelee::VP_Right:
		SetAttackDetails(VisPred::Game::VPAni::ToVP_attack_R, meleecomp.VPLength, meleecomp.VPDamage, meleecomp.SwordAngle);
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
	turncomp->MoveTime = meleecomp.Time * 0.6f;
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
			transcomp.SetLocalLocation({ 0.f, 0.f, (playercomp.Radius + (meleecomp.Length / 2)) });
			transcomp.SetLocalScale({ 0.01f, 0.01f, meleecomp.Length / 2 });
			break;
		}
	}
}
#pragma endregion
#pragma region FSM_System
#pragma region FSM Calculate

void PlayerSystem::Enter_FSM(PlayerComponent& playercomp)
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
	playercomp.PreFSM = playercomp.CurrentFSM;
}
#pragma endregion

#pragma region FSM Enter Logic
void PlayerSystem::Enter_Idle(PlayerComponent& playercomp)
{
	if (!playercomp.IsVPMode)
		if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
			DefalutModeController(playercomp);

}
void PlayerSystem::Enter_Walk(PlayerComponent& playercomp)
{
	if (!playercomp.IsVPMode)
		if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
			DefalutModeController(playercomp);
}
void PlayerSystem::Enter_Run(PlayerComponent& playercomp)
{
	if (!playercomp.IsVPMode)
		if (playercomp.PreFSM == VisPred::Game::PlayerFSM::CROUCH || playercomp.PreFSM == VisPred::Game::PlayerFSM::Dash_Slide)
			DefalutModeController(playercomp);
}
void PlayerSystem::Enter_Jump(PlayerComponent& playercomp)
{


}
void PlayerSystem::Enter_Crouch(PlayerComponent& playercomp)
{
	CrouchModeController(playercomp);
}
void PlayerSystem::Enter_Dash_Slide(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
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
void PlayerSystem::Enter_Transformation(PlayerComponent& playercomp)
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
void PlayerSystem::Enter_Die(PlayerComponent& playercomp)
{
}
void PlayerSystem::Enter_Die_end(PlayerComponent& playercomp)
{
}
#pragma endregion
#pragma endregion
#pragma region FSM Action
#pragma region FSM Action Main
void PlayerSystem::Action_FSM(PlayerComponent& playercomp, float deltaTime)
{

	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Action_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Action_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Action_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Action_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Action_Slide(playercomp, deltaTime);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Action_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Transformation:
		Action_Transformation(playercomp, deltaTime);
		break;


	case VisPred::Game::PlayerFSM::DIE:
		Action_Die(playercomp, deltaTime);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Action_Destroy(playercomp);
		break;
	default:
		break;
	}
}
#pragma endregion
#pragma region FSM Action Logic

void PlayerSystem::Action_Idle(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.InputDir = {};
	Active_Interect(playercomp);
	Active_Rotation(playercomp, transfomcomp);
	Active_Attack(playercomp);
}

void PlayerSystem::Action_Slide(PlayerComponent& playercomp, float deltatime)
{
	///지면 기준에서 하기.
	///Input이 있을경우에는 그 방향으로 이동해야함.
	/// 콜라이더 오프셋 기능 추가.
	auto& transcomp = *playercomp.GetComponent<TransformComponent>();
	Active_Slide(playercomp, deltatime);
	Active_Rotation(playercomp, transcomp);
	Active_Attack(playercomp);
}

void PlayerSystem::Action_Walk(PlayerComponent& playercomp)
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

void PlayerSystem::Action_Run(PlayerComponent& playercomp)
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
void PlayerSystem::Action_Crouch(PlayerComponent& playercomp)
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

void PlayerSystem::Action_Jump(PlayerComponent& playercomp)
{
	TransformComponent& transfomcomp = *playercomp.GetComponent<TransformComponent>();
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Active_Rotation(playercomp, transfomcomp);
	Active_Walk(transfomcomp, playercomp, Controller);
	Active_Interect(playercomp);
	Active_Jump(transfomcomp, Controller);
	Active_Attack(playercomp);
}



void PlayerSystem::Action_Die(PlayerComponent& playercomp, float deltatime)
{
	playercomp.DieProgress += deltatime;
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.InputDir = {};


}

void PlayerSystem::Action_Destroy(PlayerComponent& playercomp)
{
}

void PlayerSystem::Action_Transformation(PlayerComponent& playercomp, float deltatime)
{
	ControllerComponent& Controller = *playercomp.GetComponent<ControllerComponent>();
	Controller.InputDir = {};

	if (playercomp.TransformationProgress > playercomp.TransformationTime / 2)
		ChangeArm(playercomp, playercomp.IsVPMode);
	else
		playercomp.IsArmChanged = false;
}

#pragma endregion
#pragma endregion
#pragma region FSM Sound
#pragma region FSM Sound Main


void PlayerSystem::Sound_FSM(PlayerComponent& playercomp, float deltaTime)
{
	switch (playercomp.CurrentFSM)
	{
	case VisPred::Game::PlayerFSM::IDLE:
		Sound_Idle(playercomp);
		break;
	case VisPred::Game::PlayerFSM::WALK:
		Sound_Walk(playercomp);
		break;
	case VisPred::Game::PlayerFSM::RUN:
		Sound_Run(playercomp);
		break;
	case VisPred::Game::PlayerFSM::CROUCH:
		Sound_Crouch(playercomp);
		break;
	case VisPred::Game::PlayerFSM::Dash_Slide:
		Sound_Slide(playercomp);
		break;
	case VisPred::Game::PlayerFSM::JUMP:
		Sound_Jump(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE:
		Sound_Die(playercomp);
		break;
	case VisPred::Game::PlayerFSM::DIE_END:
		Sound_Destroy(playercomp);
		break;
	default:
		break;
	}
}
#pragma endregion
#pragma region FSM Sound Logic
void PlayerSystem::Sound_Idle(PlayerComponent& playercomp)
{
	auto& playerSoundcomp = *playercomp.GetComponent<PlayerSoundComponent>();

	playerSoundcomp.Played_Walk1 = false;
	playerSoundcomp.Played_Jump = false;
	playerSoundcomp.Played_Slide = false;
	playerSoundcomp.Played_Sit = false;
	playerSoundcomp.Played_Run1 = false;
}
void PlayerSystem::Sound_Walk(PlayerComponent& playercomp)
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
void PlayerSystem::Sound_Run(PlayerComponent& playercomp)
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
void PlayerSystem::Sound_Crouch(PlayerComponent& playercomp)
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
void PlayerSystem::Sound_Slide(PlayerComponent& playercomp)
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
void PlayerSystem::Sound_Jump(PlayerComponent& playercomp)
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

void PlayerSystem::Sound_Die(PlayerComponent& playercomp)
{
}
void PlayerSystem::Sound_Destroy(PlayerComponent& playercomp)
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
	float slidespeed{};
	if (playercomp.IsVPMode)
	{
		slidespeed = playercomp.RunSpeed * 3.f;
	}
	else
	{
		slidespeed = playercomp.RunSpeed * 1.5f;

	}

	controller.MaxSpeed = slidespeed;
	controller.Acceleration = controller.MaxSpeed * 6.f;
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
	if (playercomp.IsVPMode)
	{
		auto& anicomp = *playercomp.VPHandEntity.lock()->GetComponent<AnimationComponent>();
		if (anicomp.IsFinished)
		{
			ReturnToVPIdle(playercomp, anicomp);
		}
	}
	else
	{
		auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
		if (anicomp.IsFinished)
		{
			AnimationFinished(playercomp, anicomp);
			ReturnToIdle(anicomp);
		}
	}


}
void PlayerSystem::AnimationFinished(PlayerComponent& playercomp, AnimationComponent& anicomp)
{

	using namespace VisPred::Game;
	if (anicomp.IsBlending || !anicomp.IsFinished)
		return;
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::PlayerAni::ToAttack1_Sword:
	case  (int)VisPred::Game::PlayerAni::ToAttack2_Sword:
	case  (int)VisPred::Game::PlayerAni::ToAttack3_Sword:
	case  (int)VisPred::Game::PlayerAni::ToAttack_Pistol:
	case  (int)VisPred::Game::PlayerAni::ToAttack_Rifle:
	case  (int)VisPred::Game::PlayerAni::ToAttack_ShotGun:
	{
		playercomp.IsAttacking = false;
		break;
	}
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
void PlayerSystem::VPAnimationFinished(PlayerComponent& playercomp, AnimationComponent& anicomp)
{
	using namespace VisPred::Game;
	if (anicomp.IsBlending || !anicomp.IsFinished)
		return;
	switch (anicomp.curAni)
	{
	case  (int)VisPred::Game::VPAni::ToVP_attack_L:
	case  (int)VisPred::Game::VPAni::ToVP_attack_R:
		playercomp.IsAttacking = false;
		break;
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
		if (!INPUTKEY(MOUSEKEY::LBUTTON))
		{
			switch (playercomp.CurrentFSM)
			{
			case VisPred::Game::PlayerFSM::IDLE:	ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);	break;
			case VisPred::Game::PlayerFSM::RUN:		ChangeAni_Index(entityID, VPAni::ToVP_run, 0, 0, true);	break;
			case VisPred::Game::PlayerFSM::JUMP:	ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);	break;
			default:
				break;
			}
		}

	}
	break;
	case  (int)VisPred::Game::VPAni::ToVP_jump:		ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);	break;
	case  (int)VisPred::Game::VPAni::ToVP_draw:		ChangeAni_Index(entityID, VPAni::ToVP_Idle, 0, 0, true);	break;
	default:
		break;
	}

}
#pragma endregion
#pragma region Interect Logic
void PlayerSystem::Active_Interect(PlayerComponent& playercomp)
{
	if (playercomp.IsVPMode)
	{
		return;
	}
	if (INPUTKEYDOWN(KEYBOARDKEY::F))
	{
		Grab_Gun(playercomp);
	}
}

void PlayerSystem::Grab_Gun(PlayerComponent& playercomp)
{
	auto& anicomp = *playercomp.HandEntity.lock()->GetComponent<AnimationComponent>();
	if (anicomp.IsBlending)
		return;
	if (anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Sword
		&& anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Pistol
		&& anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_Rifle
		&& anicomp.PlayerCurAni != VisPred::Game::PlayerAni::ToIdle02_ShotGun)
		return;


	auto gunentity = GetSceneManager()->GetEntity(playercomp.SearchedItemID);
	if (!gunentity || !gunentity->HasComponent<GunComponent>())
		return;
	if (playercomp.HasGun)
	{
		Drop_Gun(playercomp);
	}


	uint32_t handID = playercomp.HandEntity.lock()->GetEntityID();

	auto guncomp = gunentity->GetComponent<GunComponent>();
	auto soceketcomp = gunentity->GetComponent<SocketComponent>();
	soceketcomp->IsConnected = true;
	soceketcomp->ConnectedEntityID = handID;
	playercomp.HasGun = true;
	playercomp.GunEntityID = guncomp->GetEntityID();
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
	auto gunentity = GetSceneManager()->GetEntity(playercomp.GunEntityID);

	auto guncomp = gunentity->GetComponent<GunComponent>();
	auto soceketcomp = gunentity->GetComponent<SocketComponent>();
	soceketcomp->IsConnected = false;
	soceketcomp->ConnectedEntityID = 0;
	playercomp.HasGun = false;
	guncomp->GetComponent<MeshComponent>()->MaskColor = {};
	guncomp->GetComponent<MeshComponent>()->IsOverDraw = false;
	///TODO 사운드 로직 추가하기.
}


#pragma endregion
#pragma region Gun Logic
bool PlayerSystem::Gun_Shoot(PlayerComponent& playercomp, GunComponent& guncomp)
{

	auto& TransformComp = *playercomp.FirePosEntity.lock()->GetComponent<TransformComponent>();
	switch (guncomp.Type)
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
	switch (guncomp.Type)
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
void PlayerSystem::Gun_RecoilingToMiddle(PlayerComponent& playercomp, float deltatime)
{
	if (playercomp.HasGun && playercomp.IsGunRecoiling)
	{
		auto& cameratrans = *playercomp.CameraEntity.lock()->GetComponent<TransformComponent>();
		playercomp.RecoilProgress += deltatime;
		auto gunComp = GetSceneManager()->GetComponent<GunComponent>(playercomp.GunEntityID);
		VPMath::Quaternion tempquat{};
		double gunprogresstime = static_cast<double>(playercomp.GunprogressTime);
		double recoilTime = static_cast<double>(gunComp->RecoilTime);
		double recoilPercent = static_cast<double>(gunComp->RecoilPercent);
		double percent = RecoilPercent(gunprogresstime, recoilTime, recoilPercent);
		if (!RecoilReturn(gunprogresstime, recoilTime, recoilPercent))
			tempquat = tempquat.Slerp(playercomp.GunRecoilStartQuat, playercomp.GunRecoilEndQuat, static_cast<float>(percent));
		else
			tempquat = tempquat.Slerp({}, playercomp.GunRecoilEndQuat, static_cast<float>(percent));
		cameratrans.SetLocalQuaternion(tempquat);

		auto& handtrans = *playercomp.HandEntity.lock()->GetComponent<TransformComponent>();
		auto temp = handtrans.Local_Location;
		temp.z = 0;
		VPMath::Vector3 temp2 = temp;
		VPMath::Vector3 temp3 = {};
		temp2.z = -0.05f;
		temp3 = temp.Lerp(temp, temp2, static_cast<float>(percent));
		handtrans.SetLocalLocation(temp3);
		if (playercomp.GunprogressTime > gunComp->RecoilTime)
		{
			VPMath::Quaternion tempquat{};
			playercomp.RecoilProgress = 0;
			playercomp.GunRecoilStartQuat = {};
			playercomp.GunRecoilEndQuat = {};
			playercomp.IsGunRecoiling = false;
			playercomp.IsAttacking = false;
			cameratrans.SetLocalQuaternion({});
			handtrans.SetLocalLocation(temp);
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
	if (idleAnimation != attackAnimation)
		ChangeAni_Index(anicomp.GetEntityID(), attackAnimation, 0, 0, false);
	else
	{
		anicomp.duration = 0;
	}
	// Update gun and player state
	playercomp.IsAttacking = true;
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
	if (!Shoot_Common(playercomp, guncomp, PlayerAni::ToIdle02_Rifle, PlayerAni::ToIdle02_Rifle))
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
	//playercomp.ShootType = VisPred::Game::GunType::NONE;
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

#pragma endregion 
#pragma region IStartable
void PlayerSystem::Initialize()
{
	m_Graphics->SetVP(false);
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
		auto LongswordEntity = GetSceneManager()->GetEntityByIdentityName(playercomp->LongswordName);
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
			float SlideHeight = static_cast<float>(((fullHeight * 0.25) - 2 * controllercomp.CapsuleControllerinfo.radius) / 2);

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
	controlcomp.JumpXZAcceleration = controlcomp.Acceleration * playercomp.AirControlPercent / 100;
	controlcomp.GravityWeight = playercomp.GravityPower * 9.80665f;
}
void PlayerSystem::UpdateControllerSize(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, controlcomp.CapsuleControllerinfo.height);
}
void PlayerSystem::CrouchModeController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	controlcomp.Contollerinfo.LocalOffset.y = -playercomp.SitHeightDiff;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, playercomp.SitHeight);
}
void PlayerSystem::SlideModeController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	controlcomp.Contollerinfo.LocalOffset.y = -playercomp.SlideHeightDiff;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, playercomp.SlideHeight);
}
void PlayerSystem::DefalutModeController(PlayerComponent& playercomp)
{
	ControllerComponent& controlcomp = *playercomp.GetComponent<ControllerComponent>();
	controlcomp.Contollerinfo.LocalOffset.y = 0;
	m_PhysicsEngine->ResizeCapsuleController(playercomp.GetEntityID(), controlcomp.CapsuleControllerinfo.radius, controlcomp.CapsuleControllerinfo.height);
}
#pragma endregion


void PlayerSystem::Finalize()
{
	m_Graphics->SetVP(false);

}

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