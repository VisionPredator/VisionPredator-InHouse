#pragma once
#include <Component.h>
#include "VisPredStructs.h"

struct GunComponent :
    public Component
{
    VP_JSONBODY(GunComponent, Type, ThrowDamage, BulletPrefab, GunSoundPrefab,CurrentBullet, CoolTime, Bullets, RecoilPos, RecoilMaxXY, RecoilPercent, RecoilTime)
    VisPred::Game::GunType Type;
    std::string BulletPrefab{};
    std::string GunSoundPrefab{};
    uint32_t CurrentBullet{};
    float CoolTime{};
    uint32_t Bullets{};
    uint32_t ThrowDamage{};
    bool IsGrapped{};
    bool IsEmpty = false;
    uint32_t GrappedEntityID{};
    VPMath::Vector2 RecoilPos{};
    VPMath::Vector2 RecoilMaxXY{};
    float RecoilPercent{};
    float RecoilTime{};
};

