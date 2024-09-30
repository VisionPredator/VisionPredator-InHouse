#pragma once
#include "Component.h"
#include "VisPredStructs.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, PlayerHandName, PlayerCameraName, CurrentFSM, HP, Sencitive, Accel, WalkSpeed, RunSpeed, SlideDisTance, StaticFriction, SlideDisTance, DynamicFriction, JumpFoce, AirControlPercent, GravityPower, FirePosition)
    std::string PlayerHandName{};
    std::string PlayerCameraName{};
    uint32_t PlayerHandID{};
    uint32_t PlayerCameraID{};
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
    float SlideDuration =0.5f;
    float SlideProgress{};
    VPMath::Vector3 SlideDir{};
    
    uint32_t SearchedItemID{};
    uint32_t PreSearchedItemID{};
    uint32_t GunEntityID{};
    uint32_t FirePosEntityID{};


    VisPred::Game::GunType ShootType{};
    bool HasGun=false;
    float GunprogressTime{};
    bool ReadyToShoot{};
};

