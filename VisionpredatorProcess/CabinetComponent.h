#pragma once
#include <Component.h>
struct CabinetComponent :
    public Component
{
    VP_JSONBODY(CabinetComponent, PistolPrefab, ShotGunPrefab, RiflePrefab, PistolPose, ShotGunPose, RiflePose, LeftDoor, RightDoor, OpenAngle, OpenTime)
    std::string LeftDoor{};
    std::string RightDoor{};
    float OpenAngle{};
    float OpenTime{};

    std::shared_ptr<Entity> RighEntity{};
    std::shared_ptr<Entity> LeftEntity{};
    std::string  PistolPrefab= "../Data/Prefab/CabinetPistol.prefab";
    std::string  ShotGunPrefab= "../Data/Prefab/CabinetShotGun.prefab";
    std::string  RiflePrefab ="../Data/Prefab/CabinetRifle.prefab";
    std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> PistolPose;
    std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> ShotGunPose;
    std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> RiflePose;
    
    bool IsOpen{};
    VPMath::Vector3 RightStartRotate = {};
    VPMath::Vector3 RightEndRotation = {};
    VPMath::Vector3 LeftStartRotate = {};
    VPMath::Vector3 LeftEndRotation = {};
    float OpenProgress{};
};

