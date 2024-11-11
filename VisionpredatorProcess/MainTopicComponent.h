#pragma once
#include <Component.h>
#include "VisPredStructs.h"
using namespace VisPred::Game;
struct MainTopicComponent :
    public Component
{
    VP_JSONBODY(MainTopicComponent, MentColor, BackGroundColor, IsReset, Duration, Mode, Ment)
    VisPred::Game::TopicType Mode;
    std::array < std::wstring, (int)VisPred::Game::TopicType::END> Ment;
    VPMath::Color MentColor{1,1,1,1};
    VPMath::Color BackGroundColor = { 0,0,0,0.5 };
    bool IsReset{};
    bool IsShowing{};
    float Duration{};
    float Progress{};

};

