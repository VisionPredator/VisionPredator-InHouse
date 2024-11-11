#pragma once
#include <Component.h>
struct SpawnerComponent :
    public Component
{
    VP_JSONBODY(SpawnerComponent, SpawnPrefab, SpawnTransform)
        bool isSpawned = false;
    std::vector<std::string> SpawnPrefab{};
    std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> SpawnTransform{};
};

