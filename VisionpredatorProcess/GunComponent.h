#pragma once
#include <Component.h>
#include "VisPredStructs.h"

struct GunComponent :
    public Component
{
    VP_JSONBODY(GunComponent, Type, CurrentBullet, Bullets)
    VisPred::Game::GunType Type;
    uint32_t CurrentBullet{};
    uint32_t Bullets{};
    uint32_t Damage{};

};

