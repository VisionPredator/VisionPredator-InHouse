#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, CurrentFSM, HP, Sencitive, Maxspeed, Accel, JumpFoce, MaxJumpCount, AirControl, HasGun, FirPosition)
		uint32_t HP{};
    VisPred::Game::EFSM CurrentFSM=VisPred::Game::EFSM::IDLE;
    float Sencitive=1.f;
    float Speed{};
    float Accel{};
    float Maxspeed{};
    float JumpFoce{};
    float JumppHeight{};
    uint32_t MaxJumpCount{};
    uint32_t Jumpcount{};
    float AirControl{};
    bool Isground{}; 
    bool HasGun{};
    std::string FirPosition;
};

