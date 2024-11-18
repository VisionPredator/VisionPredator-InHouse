#pragma once
#include <Component.h>
struct BestScoreComponent :
    public Component
{
    VP_JSONBODY(BestScoreComponent, Score, BestScoreMent, NewBestScoreMent, NewColor, BasicColor)
    std::wstring BestScoreMent = L"Best Score :";
    std::wstring NewBestScoreMent =L"New Best Score :";
    int Score = 5;
    VPMath::Color NewColor{ .7f,.7f,0.f ,1.f};
    VPMath::Color BasicColor{.7f,.7f,0.f,1.f};
};
