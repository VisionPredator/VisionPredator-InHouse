#pragma once
#include <Component.h>
struct DoorComponent :   public Component
{
    VP_JSONBODY(DoorComponent, IsOpen, OpenTime , IsUseserble, UseableFBX, UnUseableFBX, MoveDistance , LeftDoor, RightDoor)
		bool IsOpen{};
	bool IsActivated{};
	bool IsUseserble{};
	std::wstring UseableFBX = L"doorA1_outline.fbx";
	std::wstring UnUseableFBX = L"doorA2_outline.fbx";
	float OpenTime{};
	float OpenProgress{};
	float MoveDistance{};
	std::string LeftDoor{};
	std::string RightDoor{};
	std::shared_ptr<Entity> RightEntity{};
	std::shared_ptr<Entity> LeftEntity{};
	VPMath::Vector3 LeftClosePos{};
	VPMath::Vector3 RightClosePos{};
	VPMath::Vector3 LeftOpenPos{};
	VPMath::Vector3 RightOpenPos{};
};

