#pragma once
#include <Component.h>
struct EntityRemoverComponet :
    public Component
{
    VP_JSONBODY(EntityRemoverComponet, temp)
        std::vector<uint32_t> RemoveEntitys{};
    bool temp{};
};

