#pragma once
#include <Component.h>
struct WeaponBoxComponent :
    public Component
{
    VP_JSONBODY(WeaponBoxComponent, SpawnPrefabs, TopEntity, SpawnOffset, SpawnSpeed, SpawnDirection, OpenDegree, OpenTime)
        bool IsOpen = false;
    std::string TopEntity{  };
    std::vector<std::string> SpawnPrefabs{};
    VPMath::Vector3 SpawnOffset{};
    VPMath::Vector3 SpawnPos{};
    VPMath::Vector3 SpawnDirection{};
    VPMath::Vector3 StartRotation = {};
    VPMath::Vector3 EndRotation = {};
    float SpawnSpeed{};
    float OpenDegree{};
    float OpenTime{};
    float OpenProgress{};
};

