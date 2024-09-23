#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, CurrentFSM, HP, Sencitive, Accel, WalkSpeed, RunSpeed, SlideDisTance, StaticFriction, SlideDisTance, DynamicFriction, JumpFoce, AirControlPercent, HasGun, GravityPower, FirePosition)
        uint32_t HP{};
    VisPred::Game::EFSM CurrentFSM = VisPred::Game::EFSM::IDLE;
    float Height{};
    float Radius{};
    float Sencitive = 1.f;
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

    /// <summary>
    /// 게임로직을 위한 변수들
    /// </summary>
    std::string FirePosition;
    VPMath::Vector3 SitCameraPos{};
    float SitHeight{};
    float SitHeightDiff{};
    float CamTransDuration=0.07f;
    float CamTransProgress=0.f;
    VPMath::Vector3 DefalutCameraPos{};
    float SlideDuration{};
    float SlideProgress{};
};

