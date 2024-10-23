#include "pch.h"
#include "DataRegister_Vispred.h"
#include "VisPredComponents.h"
#include "VisPredStructs.h"
void VispredRegister::Register_Metadata()
{
	VispredRegister::Register_Components();
	VispredRegister::Register_EnumClass();
	VispredRegister::Register_Structs();
	VispredRegister::Register_Value();
	VispredRegister::Register_VPMath();
}

void VispredRegister::Register_Components()
{
	META_ADD_COMP(BulletComponent, BulletComponent::Damage, BulletComponent::Speed);
	META_ADD_COMP(PlayerComponent, PlayerComponent::RecoilMode,
		PlayerComponent::CameraPosName,PlayerComponent::CameraName, PlayerComponent::FirePosName, PlayerComponent::HandName,PlayerComponent::LongswordName,
		PlayerComponent::MaxHP,	PlayerComponent::HP
		, PlayerComponent::Sencitive, PlayerComponent::RecoilReturnTime
		, PlayerComponent::JumpForce
		, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed, PlayerComponent::SlideDuration
		,  PlayerComponent::CurrentFSM
		, PlayerComponent::AirControlPercent
		, PlayerComponent::GravityPower
		, PlayerComponent::StaticFriction
		, PlayerComponent::DynamicFriction

	);
	META_ADD_COMP(PlayerSoundComponent, PlayerSoundComponent::WalkSoundKey1, PlayerSoundComponent::WalkSoundKey2, PlayerSoundComponent::Volume_Walk
		, PlayerSoundComponent::RunSoundKey1, PlayerSoundComponent::RunSoundKey2, PlayerSoundComponent::Volume_Run
		, PlayerSoundComponent::JumpSoundkey, PlayerSoundComponent::Volume_Jump
		, PlayerSoundComponent::SlideSoundkey, PlayerSoundComponent::Volume_Slide
		, PlayerSoundComponent::SitSoundKey, PlayerSoundComponent::Volume_Sit
		, PlayerSoundComponent::HurtSoundKey, PlayerSoundComponent::Volume_Hurt
	);
		META_ADD_COMP(TrunComponent, TrunComponent::MoveTime, TrunComponent::Is_X, TrunComponent::Angle, TrunComponent::Finished);
		META_ADD_COMP(AreaAttackComponent, AreaAttackComponent::Damage);
		META_ADD_COMP(PlayerMeleeComponent, PlayerMeleeComponent::DefalutPrefab, PlayerMeleeComponent::AttackMode, PlayerMeleeComponent::SwordLength, PlayerMeleeComponent::SwordAngle, PlayerMeleeComponent::SwordDamage, PlayerMeleeComponent::VPLength, PlayerMeleeComponent::VPAngle, PlayerMeleeComponent::VPDamage);

	META_ADD_COMP(EnemyComponent, EnemyComponent::HP, EnemyComponent::CurrentFSM, EnemyComponent::HorizontalFOV, EnemyComponent::VerticalFOV, EnemyComponent::NearZ, EnemyComponent::FarZ, EnemyComponent::IsModelFlipped);
	META_ADD_COMP(GunComponent, GunComponent::Type,GunComponent::ThrowDamage, GunComponent::BulletPrefab,GunComponent::GunSoundPrefab, GunComponent::CoolTime, GunComponent::CurrentBullet, GunComponent::Bullets,GunComponent::RecoilPos, GunComponent::RecoilMaxXY, GunComponent::RecoilTime, GunComponent::RecoilPercent);
}

void VispredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(EFSM, EFSM::ATTACK, EFSM::DESTROY, EFSM::DIE, EFSM::IDLE, EFSM::JUMP, EFSM::WALK, EFSM::RUN, EFSM::CROUCH,EFSM::SLIDE, EFSM::NONE);
	META_ADD_ENUMCLASS(PlayerMelee, PlayerMelee::Sword_First, PlayerMelee::Sword_Second, PlayerMelee::Sword_Third, PlayerMelee::Sword_Fourth, PlayerMelee::VP_Left, PlayerMelee::VP_Right);
	META_ADD_ENUMCLASS(GunRecoilMode, GunRecoilMode::ReturnToEndAim, GunRecoilMode::ReturnToMiddle);
	META_ADD_ENUMCLASS(EnemyStates, EnemyStates::Idle, EnemyStates::Chase, EnemyStates::Patrol, EnemyStates::Dead);
	META_ADD_ENUMCLASS(EnemyAni, EnemyAni::ATTACK, EnemyAni::IDLE, EnemyAni::CHASE, EnemyAni::JUMP, EnemyAni::WALK, EnemyAni::DIE, EnemyAni::ATTACKED, EnemyAni::BACKWALK);
	META_ADD_ENUMCLASS(GunType, GunType::NONE, GunType::PISTOL, GunType::RIFLE, GunType::SHOTGUN, GunType::END);
	META_ADD_ENUMCLASS(PlayerAni
		, PlayerAni::ToAttack_Pistol
		,PlayerAni::ToAttack_Rifle
		,PlayerAni::ToAttack_ShotGun
		,PlayerAni::ToIdle01_Pistol
		,PlayerAni::ToIdle01_Rifle
		,PlayerAni::ToIdle01_ShotGun
		,PlayerAni::ToIdle02_Pistol
		,PlayerAni::ToIdle02_Rifle
		,PlayerAni::ToIdle02_ShotGun
		,PlayerAni::Tohook_Sword
		,PlayerAni::Tohook_Pistol
		,PlayerAni::Tohook_Rifle
		,PlayerAni::Tohook_ShotGun
		,PlayerAni::Tointeraction
		,PlayerAni::ToAttack1_Sword
		,PlayerAni::ToAttack2_Sword
		,PlayerAni::ToAttack3_Sword
		,PlayerAni::ToIdle01_Sword
		,PlayerAni::ToIdle02_Sword
		,PlayerAni::ToThrow_Pistol
		,PlayerAni::ToThrow_Rifle
		,PlayerAni::ToThrow_ShotGun
		,PlayerAni::ToVP_attack_L
		,PlayerAni::ToVP_attack_R
		,PlayerAni::ToVP_Idle
		,PlayerAni::ToVP_dash
		,PlayerAni::ToVP_jump
		,PlayerAni::ToVP_run
		,PlayerAni::ToVP_draw);
}

void VispredRegister::Register_Structs()
{
	
}

void VispredRegister::Register_Value()
{
}

void VispredRegister::Register_VPMath()
{
}
