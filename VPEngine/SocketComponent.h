#pragma once
#include "Component.h"
struct SocketComponent :    public Component
{
	VP_JSONBODY(SocketComponent, ConnectedEntity, SocketName, IsConneted, Offset, Rotation)
	std::string ConnectedEntity{};
	std::string PreviewConnectedEntity{};
	uint32_t ConnectedEntityID{};
	std::wstring SocketName{};
	bool IsConneted{};
	VPMath::Vector3 Offset{};
	VPMath::Vector3 Rotation{};
};

