#pragma once
#include "Component.h"
using namespace entt::literals;


/// <summary>
/// 나중에 prefab이 계층관계가있다면, 어찌하지?
/// </summary>


struct Parent : public Component
{
	VP_JSONBODY(Parent, ParentID)
		Parent();

	uint32_t ParentID = 0;


};

struct Children : public Component
{
	VP_JSONBODY(Children, ChildrenID)
		Children();

	std::list<uint32_t> ChildrenID;

};

struct TransformComponent :public Component
{
public:

	VP_JSONBODY(TransformComponent, Local_Location, Local_Quaternion, Local_Scale)
	TransformComponent();
	VPMath::Vector3 Local_Location = {};
	VPMath::Vector3 Local_Rotation = {};
	VPMath::Quaternion Local_Quaternion = {};
	VPMath::Vector3 Local_Scale = { 1.f,1.f,1.f };

	VPMath::Vector3 Previous_Location = {};
	VPMath::Vector3 Previous_Rotation = {};
	VPMath::Quaternion Previous_Quaternion = {};
	VPMath::Vector3 Previous_Scale = { 1.f,1.f,1.f };

	VPMath::Vector3 Previous_WorldLocation = {};
	VPMath::Vector3 Previous_WorldRotation = {};
	VPMath::Quaternion Previous_WorldQuaternion = {};
	VPMath::Vector3 Previous_WorldScale = { 1.f,1.f,1.f };
	VPMath::Matrix Previous_WorldTransform = {};

	VPMath::Vector3 World_Location = {};
	VPMath::Vector3 World_Rotation = {};
	VPMath::Quaternion World_Quaternion = {};
	VPMath::Vector3 World_Scale = { 1.f,1.f,1.f };
	VPMath::Vector3 FrontVector = {};
	VPMath::Vector3 UpVector = {};
	VPMath::Vector3 RightVector = {};
	VPMath::Matrix LocalTransform = {}; 
	VPMath::Matrix WorldTransform = {};
	void SetLocalLocation(VPMath::Vector3);
	void SetLocalQuaternion(VPMath::Quaternion);
	void SetLocalRotation(VPMath::Vector3);
	void SetLocalScale(VPMath::Vector3);
	void SetWorldLocation(VPMath::Vector3);
	void SetWorldQuaternion(VPMath::Quaternion);
	void SetWorldRotation(VPMath::Vector3);
	void SetWorldScale(VPMath::Vector3);
};
