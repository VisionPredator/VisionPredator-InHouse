#pragma once
#include "Component.h"
struct SocketComponent :    public Component
{
	VP_JSONBODY(SocketComponent, ConnectedEntity, SocketName, IsConnected, Offset, UseQuaternion, OffsetRotation, OffsetQuaternion)
	std::string ConnectedEntity{};
	std::string PreviewConnectedEntity{};
	uint32_t ConnectedEntityID{};
	uint32_t PreConnectedEntityID{};
	std::wstring SocketName{};
	bool IsConnected{};
	VPMath::Vector3 Offset{};
	bool UseQuaternion{};
	VPMath::Vector3 OffsetRotation{};
	VPMath::Quaternion OffsetQuaternion{};
	VPMath::Matrix AttachmentMatrix{};
};

