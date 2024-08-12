#include "pch.h"
#include "TransformComponent.h"
#include "TransformSystem.h"




	TransformComponent::TransformComponent()
	{
		FrontVector = WorldTransform.Forward();
		UpVector = WorldTransform.Up();
	}

	Parent::Parent()
	{
	}

	Children::Children()
	{
	}

	void TransformComponent::SetLocalLocation(VPMath::Vector3 newlocal)
	{
		if (Local_Location == newlocal)
			return;
		Local_Location = newlocal;
		TransformSystem::AddUpdateData(this);
	}
	void TransformComponent::SetLocalQuaternion(VPMath::Quaternion newlocal)
	{
		if (Local_Quaternion == newlocal)
			return;
		Local_Quaternion = newlocal;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetLocalRotation(VPMath::Vector3 newlocal)
	{
		if (Local_Rotation == newlocal)
			return;
		Local_Rotation = newlocal;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetLocalScale(VPMath::Vector3 newlocal)
	{
		if (Local_Scale == newlocal)
			return;
		Local_Scale = newlocal;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldLocation(VPMath::Vector3 newworld)
	{
		if (World_Location == newworld)
			return;
		World_Location = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldQuaternion(VPMath::Quaternion newworld)
	{
		if (World_Quaternion == newworld)
			return;
		World_Quaternion = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldRotation(VPMath::Vector3 newworld)
	{
		if (World_Rotation == newworld)
			return;
		World_Rotation = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldScale(VPMath::Vector3 newworld)
	{
		if (World_Scale == newworld)
			return;
		World_Scale = newworld;
		TransformSystem::AddUpdateData(this);
	}
