#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
	VP_JSONBODY(PlayerComponent, CurrentFSM, HP, Sencitive, Accel, WalkSpeed, RunSpeed, SlideDisTance, StaticFriction, SlideDisTance, DynamicFriction, JumpFoce, AirControlPercent, HasGun, GravityPower, FirPosition)
	uint32_t HP{};
    VisPred::Game::EFSM CurrentFSM=VisPred::Game::EFSM::IDLE;
    float Sencitive=1.f;
    float WalkSpeed{};
    float RunSpeed{};
    float SlideDisTance{};
    float Accel{};
    float StaticFriction{};
    float DynamicFriction{};
    float JumpFoce{};
    float AirControlPercent{};
    float GravityPower = 1.f;
    bool HasGun{};
    VisPred::Game::GunType ShootType{};
    std::string FirPosition;
};

