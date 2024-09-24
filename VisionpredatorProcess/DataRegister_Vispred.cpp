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
	META_ADD_COMP(PlayerComponent, PlayerComponent::HP, PlayerComponent::Sencitive, PlayerComponent::StaticFriction, PlayerComponent::DynamicFriction, PlayerComponent::JumpFoce, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed, PlayerComponent::Accel, PlayerComponent::CurrentFSM, PlayerComponent::AirControlPercent, PlayerComponent::FirePosition, PlayerComponent::GravityPower, PlayerComponent::HasGun);
	META_ADD_COMP(EnemyComponent, EnemyComponent::HP, EnemyComponent::CurrentFSM);
}

void VispredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(EFSM, EFSM::ATTACK, EFSM::CHARGE, EFSM::DESTROY, EFSM::DIE, EFSM::IDLE, EFSM::JUMP, EFSM::WALK, EFSM::RUN, EFSM::CROUCH,EFSM::SLIDE, EFSM::NONE);
	META_ADD_ENUMCLASS(EnemyState,EnemyState::IDLE, EnemyState::ATTACK, EnemyState::DIE, EnemyState::DESTROY);
	META_ADD_ENUMCLASS(GunType, GunType::NONE, GunType::PISTOL, GunType::RIFLE, GunType::SHOTGUN, GunType::END);
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
