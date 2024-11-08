#pragma once
#include <Component.h>
struct WeaponBoxComponent :
    public Component
{
    VP_JSONBODY(WeaponBoxComponent, SpawnPrefabs, TopMesh, SpawnOffset, SpawnSpeed, SpawnDirection, OpenAngle, OpenTime)
        bool IsOpen = false;
    std::string TopMesh{  };
    std::shared_ptr<Entity> TopEntity{  };
    std::vector<std::string> SpawnPrefabs{};
    VPMath::Vector3 SpawnOffset{};
    VPMath::Vector3 SpawnPos{};
    VPMath::Vector3 SpawnDirection{};
    VPMath::Vector3 StartRotation = {};
    VPMath::Vector3 EndRotation = {};
    float SpawnSpeed{};
    float OpenAngle{};
    float OpenTime{};
    float OpenProgress{};
};

