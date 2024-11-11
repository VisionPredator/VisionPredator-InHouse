#pragma once
#include "Component.h"
struct SpawnChildComponent : public Component
{
	VP_JSONBODY(SpawnChildComponent, Prefab1, Prefab2, Prefab3, Prefab4, Prefab5, Prefab1_Pose, Prefab2_Pose, Prefab3_Pose, Prefab4_Pose, Prefab5_Pose)
		bool IsSpwaned = false;
	std::string Prefab1{};
	std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> Prefab1_Pose;
	std::string Prefab2{};
	std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> Prefab2_Pose;
	std::string Prefab3{};
	std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> Prefab3_Pose;
	std::string Prefab4{};
	std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> Prefab4_Pose;
	std::string Prefab5{};
	std::vector<std::tuple<VPMath::Vector3, VPMath::Vector3, VPMath::Vector3>> Prefab5_Pose;

};

