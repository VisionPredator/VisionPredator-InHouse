#pragma once
#include "Component.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, Sencitive, HP, Maxspeed, Accel, JumpFoce, MaxJumpCount, AirControl, HasGun, FirPosition)
		uint32_t HP{};
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
