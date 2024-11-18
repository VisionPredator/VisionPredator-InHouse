#pragma once
#include <Component.h>
#include "InputManager.h"
struct DeletePrefabComponent :
    public Component
{
    VP_JSONBODY(DeletePrefabComponent, EntityIdentity, DestoryOne, DestoryAll)
    std::string EntityIdentity;
    KEYBOARDKEY DestoryOne= KEYBOARDKEY::F6;
    KEYBOARDKEY DestoryAll= KEYBOARDKEY::F6;
};

