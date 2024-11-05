#pragma once
#include <Component.h>
struct CabinetComponent :
    public Component
{
    VP_JSONBODY(CabinetComponent, LeftDoor, RightDoor, OpenAngle, OpenTime)
    std::string LeftDoor{};
    std::string RightDoor{};
    float OpenAngle{};
    float OpenTime{};

    std::shared_ptr<Entity> RighEntity{};
    std::shared_ptr<Entity> LeftEntity{};


    bool IsOpen{};
    VPMath::Vector3 RightStartRotate = {};
    VPMath::Vector3 RightEndRotation = {};
    VPMath::Vector3 LeftStartRotate = {};
    VPMath::Vector3 LeftEndRotation = {};
    float OpenProgress{};
};

