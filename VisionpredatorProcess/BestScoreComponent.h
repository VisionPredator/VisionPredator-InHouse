#pragma once
#include <Component.h>
struct BestScoreComponent :
    public Component
{
    VP_JSONBODY(BestScoreComponent, Score, BestScoreMent, NewBestScoreMent, NewColor, BasicColor)
    std::string BestScoreMent = "Best Score";
    std::string NewBestScoreMent ="New Best Score";
    int Score{};
    VPMath::Color NewColor{};
    VPMath::Color BasicColor{};
};

