#pragma once
#include"InputManager.h"
#include <Component.h>
struct SceneChangeComponent :
    public Component
{
    VP_JSONBODY(SceneChangeComponent, ResetButton, ScenePath, SceneChangeable)
        bool SceneChangeable = true;
    std::string ScenePath{};
    KEYBOARDKEY ResetButton = KEYBOARDKEY::P;
};

