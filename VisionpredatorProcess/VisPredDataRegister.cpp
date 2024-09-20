#include "pch.h"
#include "VisPredDataRegister.h"
#include "VisPredComponents.h"
#include "VisPredStructs.h"
void VisPredRegister::Register_Metadata()
{
	VisPredRegister::Register_Components();
	VisPredRegister::Register_EnumClass();
	VisPredRegister::Register_Structs();
	VisPredRegister::Register_Value();
	VisPredRegister::Register_VPMath();
}

void VisPredRegister::Register_Components()
{
	META_ADD_COMP(BulletComponent, BulletComponent::Damage, BulletComponent::Speed);
	META_ADD_COMP(PlayerComponent, PlayerComponent::HP, PlayerComponent::Sencitive, PlayerComponent::StaticFriction, PlayerComponent::DynamicFriction, PlayerComponent::JumpFoce, PlayerComponent::WalkSpeed, PlayerComponent::RunSpeed, PlayerComponent::Accel, PlayerComponent::CurrentFSM, PlayerComponent::AirControlPercent, PlayerComponent::FirPosition, PlayerComponent::GravityPower, PlayerComponent::HasGun);
	META_ADD_COMP(EnemyComponent, EnemyComponent::HP, EnemyComponent::CurrentFSM);
}

void VisPredRegister::Register_EnumClass()
{
	using namespace VisPred::Game;
	META_ADD_ENUMCLASS(EFSM, EFSM::ATTACK, EFSM::CHARGE, EFSM::DESTROY, EFSM::DIE, EFSM::IDLE, EFSM::JUMP, EFSM::WALK, EFSM::RUN, EFSM::CROUCH,EFSM::SLIDE, EFSM::NONE);
	META_ADD_ENUMCLASS(GunType, GunType::NONE, GunType::PISTOL, GunType::RIFLE, GunType::SHOTGUN, GunType::END);
}

void VisPredRegister::Register_Structs()
{
	
}

void VisPredRegister::Register_Value()
{
}

void VisPredRegister::Register_VPMath()
{
}
