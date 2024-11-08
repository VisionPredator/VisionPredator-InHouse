#pragma once
#include <Component.h>
struct ShotGunBulletComponent :
    public Component
{
    VP_JSONBODY(ShotGunBulletComponent, SparkParticleName, BloodSplashParticleName, Damage1, Damage2, Damage3,Distance,Speed)

    int Damage1{};
    int Damage2{};
    int Damage3{};
    float Distance{};
    float Speed{};
    VPMath::Vector3 StartPoint{};

    std::string SparkParticleName{};
    std::string BloodSplashParticleName{};
};

