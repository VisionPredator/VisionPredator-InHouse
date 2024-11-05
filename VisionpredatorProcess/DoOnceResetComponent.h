#pragma once
#include <Component.h>
struct DoOnceResetComponent :
    public Component
{
    VP_JSONBODY(DoOnceResetComponent, ResetIdentitys)
    std::vector<std::string> ResetIdentitys;



};

