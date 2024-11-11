#pragma once
#include <Component.h>
struct SpawnSoundComponent:
    public Component
{
    VP_JSONBODY(SpawnSoundComponent, SoundKey_Volume_2D_Loop);
    std::vector<std::tuple<std::string, int, bool, bool>> SoundKey_Volume_2D_Loop;
};

