#include "pch.h"
#include "TransformComponent.h"
#include "TransformSystem.h"




	TransformComponent::TransformComponent()
	{
		FrontVector = WorldTransform.Forward_L();
		UpVector = WorldTransform.Up_L();
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

	void TransformComponent::AddWorldLocation(VPMath::Vector3 addworld)
	{
		if (addworld == VPMath::Vector3::Zero)
			return;
		World_Location += addworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldLocation(VPMath::Vector3 newworld)
	{
		if (World_Location == newworld)
			return;
		World_Location = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldLocationX(float newworld)
	{
		if (World_Location.x == newworld)
			return;
		World_Location.x = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldLocationY(float newworld)
	{
		if (World_Location.y == newworld)
			return;
		World_Location.y = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldLocationZ(float newworld)
	{
		if (World_Location.z == newworld)
			return;
		World_Location.z = newworld;
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
	void TransformComponent::SetWorldRotationX(float newworld)
	{										   
		if (World_Rotation.x == newworld)		   
			return;							   
		World_Rotation.x = newworld;			   
		TransformSystem::AddUpdateData(this);  
	}										   
	void TransformComponent::SetWorldRotationY(float newworld)
	{										   
		if (World_Rotation.y == newworld)		   
			return;							   
		World_Rotation.y = newworld;			   
		TransformSystem::AddUpdateData(this);  
	}										   
	void TransformComponent::SetWorldRotationZ(float newworld)
	{
		if (World_Rotation.z == newworld)
			return;
		World_Rotation.z = newworld;
		TransformSystem::AddUpdateData(this);
	}
	void TransformComponent::SetWorldScale(VPMath::Vector3 newworld)
	{
		if (World_Scale == newworld)
			return;
		World_Scale = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldScaleX(float newworld)
	{
		if (World_Scale.x == newworld)
			return;
		World_Scale.x = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldScaleY(float newworld)
	{
		if (World_Scale.y == newworld)
			return;
		World_Scale.y = newworld;
		TransformSystem::AddUpdateData(this);
	}

	void TransformComponent::SetWorldScaleZ(float newworld)
	{
		if (World_Scale.z == newworld)
			return;
		World_Scale.z = newworld;
		TransformSystem::AddUpdateData(this);
	}
