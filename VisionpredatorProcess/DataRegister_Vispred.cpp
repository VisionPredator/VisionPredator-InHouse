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
	META_ADD_COMP(PlayerComponent, 
		PlayerComponent::CameraPosName,PlayerComponent::CameraName, PlayerComponent::FirePosName, PlayerComponent::HandName,
		PlayerComponent::HP
		, PlayerComponent::Sencitive, PlayerComponent::RecoilReturnTime
		, PlayerComponent::JumpForce
		, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed, PlayerComponent::SlideDuration
		,  PlayerComponent::CurrentFSM
		, PlayerComponent::AirControlPercent
		, PlayerComponent::GravityPower
		, PlayerComponent::StaticFriction
		, PlayerComponent::DynamicFriction
		, PlayerComponent::WalkSoundKey1, PlayerComponent::WalkSoundKey2, PlayerComponent::Volume_Walk
		, PlayerComponent::RunSoundKey1, PlayerComponent::RunSoundKey2, PlayerComponent::Volume_Run
		, PlayerComponent::JumpSoundkey, PlayerComponent::Volume_Jump
		, PlayerComponent::SlideSoundkey, PlayerComponent::Volume_Slide
		, PlayerComponent::SitSoundKey, PlayerComponent::Volume_Sit
		, PlayerComponent::HurtSoundKey, PlayerComponent::Volume_Hurt
	);
	META_ADD_COMP(EnemyComponent, EnemyComponent::HP, EnemyComponent::CurrentFSM);
	META_ADD_COMP(GunComponent, GunComponent::Type,GunComponent::ThrowDamage, GunComponent::BulletPrefab,GunComponent::GunSoundPrefab, GunComponent::CoolTime, GunComponent::CurrentBullet, GunComponent::Bullets,GunComponent::RecoilPos, GunComponent::RecoilMaxXY, GunComponent::RecoilTime, GunComponent::RecoilPercent);
}

void VispredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(EFSM, EFSM::ATTACK, EFSM::DESTROY, EFSM::DIE, EFSM::IDLE, EFSM::JUMP, EFSM::WALK, EFSM::RUN, EFSM::CROUCH,EFSM::SLIDE, EFSM::NONE);
	META_ADD_ENUMCLASS(EnemyState,EnemyState::IDLE, EnemyState::ATTACK, EnemyState::DIE, EnemyState::DESTROY);
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
