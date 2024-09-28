#pragma once
#include <Component.h>
#include "VisPredStructs.h"

struct GunComponent :
    public Component
{
    VP_JSONBODY(GunComponent, Type, BulletPrefab,CurrentBullet, CoolTime, Bullets)
    VisPred::Game::GunType Type;
    std::string BulletPrefab{};
    uint32_t CurrentBullet{};
    float CoolTime{};
    uint32_t Bullets{};
    uint32_t Damage{};
    bool IsGrapped{};
    bool IsEmpty = false;
    uint32_t GrappedEntityID{};
};

