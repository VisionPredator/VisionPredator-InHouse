#pragma once
#include <Component.h>
struct BestScoreComponent :
    public Component
{
    VP_JSONBODY(BestScoreComponent, Score, BestScoreMent, NewBestScoreMent, NewColor, BasicColor)
    std::wstring BestScoreMent = L"Best Score :";
    std::wstring NewBestScoreMent =L"New Best Score :";
    int Score = 5;
    VPMath::Color NewColor{ .7,.7,0 ,1};
    VPMath::Color BasicColor{.7,.7,0,1};
};
