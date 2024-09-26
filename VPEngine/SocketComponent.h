#pragma once
#include "Component.h"
struct SocketComponent :    public Component
{
	VP_JSONBODY(SocketComponent, ConnectedEntity, SocketName, IsConnected, Offset, OffsetRotation)
	std::string ConnectedEntity{};
	std::string PreviewConnectedEntity{};
	uint32_t ConnectedEntityID{};
	uint32_t PreConnectedEntityID{};
	std::wstring SocketName{};
	bool IsConnected{};
	VPMath::Vector3 Offset{};
	VPMath::Vector3 OffsetRotation{};
};

