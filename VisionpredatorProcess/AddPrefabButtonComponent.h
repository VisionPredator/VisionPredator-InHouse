#pragma once
#include <Component.h>
#include "InputManager.h"
struct AddPrefabButtonComponent :
    public Component
{
    VP_JSONBODY(AddPrefabButtonComponent, Button, Prefabname, Prefab_Pose)
        KEYBOARDKEY Button= KEYBOARDKEY::F1;
    std::string Prefabname;
    std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3> Prefab_Pose;

};

