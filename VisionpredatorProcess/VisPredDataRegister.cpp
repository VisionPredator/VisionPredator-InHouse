#include "pch.h"
#include "VisPredDataRegister.h"
#include "VisPredComponents.h"
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
	META_ADD_COMP(PlayerComponent, PlayerComponent::Sencitive, PlayerComponent::HP, PlayerComponent::Maxspeed, PlayerComponent::Speed, PlayerComponent::Accel, PlayerComponent::Maxspeed, PlayerComponent::JumpFoce, PlayerComponent::MaxJumpCount, PlayerComponent::Jumpcount, PlayerComponent::Isground, PlayerComponent::HasGun,PlayerComponent::FirPosition);

}

void VisPredRegister::Register_EnumClass()
{
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
