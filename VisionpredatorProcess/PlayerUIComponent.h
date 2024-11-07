#pragma once
#include <Component.h>
struct PlayerUIComponent :
    public Component
{
    VP_JSONBODY(PlayerUIComponent, Player,FadeUI, HPGage, AimUI, VPeyeUI, WeaponUI, HitUI, InterectionUI)
        std::string Player = "Player";
    std::string FadeUI = "FadeUI";
    std::string HPGage="HPGageUI";
    std::string AimUI= "AimUI";
    std::string VPeyeUI= "VPeyeUI";
    std::string WeaponUI= "WeaponUI";
    std::string HitUI = "HitUI";
    std::string InterectionUI = "InterectionUI";

    std::weak_ptr<Entity> PlayerEntity;
    std::weak_ptr<Entity> FadeEntity;
    std::weak_ptr<Entity> HPEntity;
    std::weak_ptr<Entity> AimEntity;
    std::weak_ptr<Entity> VPeyeEntity;
    std::weak_ptr<Entity> WeaponEntity;
    std::weak_ptr<Entity> HitEntity;
    std::weak_ptr<Entity> InterectionEntity;
};

