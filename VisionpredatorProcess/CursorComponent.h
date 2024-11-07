#pragma once
#include <Component.h>
struct CursorComponent :
    public Component
{
    VP_JSONBODY(CursorComponent, ShowCursor, CursorImage, CursorScale)
		bool ShowCursor{};
    bool preIsCursor{};
    std::string CursorImage{};
    float CursorScale =1;
};

