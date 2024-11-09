#pragma once
#include <Component.h>
#include "VisPredStructs.h"

struct GunComponent :
    public Component
{
	VP_JSONBODY(GunComponent, Type, ThrowDamage, Damage1, Damage2, Damage3, BulletSpeed, BulletSize, ShotGunDistance, BulletPrefab, MuzzleEffectPointLightPrefab, SoundKey_GunSound, SoundKey_GunDrop, Volume_GunDrop, Volume_GunSound, CurrentBullet, CoolTime, Bullets, RecoilPos, RecoilMaxXY, RecoilBack, RecoilPercent, RecoilTime)
        VisPred::Game::GunType Type;

    std::weak_ptr<Entity> SoundEntity{};
    std::string BulletPrefab{};
    std::string MuzzleEffectPointLightPrefab{};
    std::string SoundKey_GunSound{};
    std::string  SoundKey_GunDrop{};
    int  Volume_GunDrop{};
    int Volume_GunSound{};
    uint32_t CurrentBullet =10;
    uint32_t Bullets{};
    float CoolTime{};
    int ThrowDamage{};
    int Damage1{};
    int Damage2{};
    int Damage3{};
    VPMath::Vector2 BulletSize{};
    float BulletSpeed{};
    float ShotGunDistance{};
    bool IsGrapped{};
    bool IsEmpty = false;
    uint32_t GrappedEntityID{};
    VPMath::Vector2 RecoilPos{};
    VPMath::Vector2 RecoilMaxXY{};
    float RecoilBack{};
    float RecoilPercent{};
    float RecoilTime{};
};

