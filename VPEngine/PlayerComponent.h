#pragma once
#include "Component.h"
struct PlayerComponent :
    public Component
{
    PlayerComponent();
    VP_JSONBODY(PlayerComponent, HP)
    uint32_t HP;

};

