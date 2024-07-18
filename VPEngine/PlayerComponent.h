#pragma once
#include "Component.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, HP, Maxspeed, Accel, JumpFoce, MaxJumpCount, AirControl)
		uint32_t HP{};
    float Speed{};
    float Accel{};
    float Maxspeed{};
    float JumpFoce{};
    float JumppHeight{};
    uint32_t MaxJumpCount{};
    uint32_t Jumpcount{};
    float AirControl{};
    bool Isground{};

};

