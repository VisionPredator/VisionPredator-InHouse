#pragma once
#include "Component.h"
using namespace entt::literals;

namespace VisPred
{
	/// <summary>
/// 나중에 prefab이 계층관계가있다면, 어찌하지?
/// </summary>


	struct Parent : public Component
	{
		VP_JSONBODY(Parent, ParentID)
		uint32_t ParentID = 0;


	};

	struct Children : public Component
	{
		VP_JSONBODY(Children, ChildrenID)

		std::list<uint32_t> ChildrenID;

	};

	struct TransformComponent :public Component
	{
		TransformComponent();
		VP_JSONBODY(TransformComponent, Local_Location, Local_Quaternion, Local_Scale)


		VPMath::Vector3 Local_Location = {};
		VPMath::Quaternion Local_Quaternion = {};
		VPMath::Vector3 Local_Scale = { 1.f,1.f,1.f };

		VPMath::Vector3 Previous_Location = {};
		VPMath::Quaternion Previous_Quaternion = {};
		VPMath::Vector3 Previous_Scale = { 1.f,1.f,1.f };

		VPMath::Vector3 World_Location = {};
		VPMath::Quaternion World_Quaternion = {};
		VPMath::Vector3 World_Scale = { 1.f,1.f,1.f };

		VPMath::Matrix LocalTransform = {};
		VPMath::Matrix WorldTransform = {};
	};
}
