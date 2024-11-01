#pragma once
#include <Component.h>
///RigidbodyTrigger ÇÊ¼ö!!
struct VPDetectionComponent :
    public Component
{
    VP_JSONBODY(VPDetectionComponent, EnemyColor, InterectColor, Length);
    std::shared_ptr<Entity> PlayerEntity{};
    VPMath::Color EnemyColor{};
    VPMath::Color InterectColor{};
    std::list<uint32_t> EnteredEnemy;
    std::list<uint32_t> InterecableEntity;
    float Length{};
    bool IsDectionMode{};

};

