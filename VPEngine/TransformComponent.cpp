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

	bool TransformComponent::SetLocalLocation(VPMath::Vector3 newlocal)
	{
		if (Local_Location == newlocal)
			return false;
		Local_Location = newlocal;
		TransformSystem::AddUpdateData(this);
		return true;

	}
	bool TransformComponent::SetLocalQuaternion(VPMath::Quaternion newlocal)
	{
		if (Local_Quaternion == newlocal)
			return false;
		Local_Quaternion = newlocal;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetLocalRotation(VPMath::Vector3 newlocal)
	{
		if (Local_Rotation == newlocal)
			return false;
		Local_Rotation = newlocal;
		TransformSystem::AddUpdateData(this);
		return true;
	}

	bool TransformComponent::SetLocalScale(VPMath::Vector3 newlocal)
	{
		if (Local_Scale == newlocal)
			return false;
		Local_Scale = newlocal;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::AddWorldLocation(VPMath::Vector3 addworld)
	{
		if (addworld == VPMath::Vector3::Zero)
			return false;
		World_Location += addworld;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetWorldLocation(VPMath::Vector3 newworld)
	{
		if (World_Location == newworld)
			return false;
		World_Location = newworld;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetWorldLocationX(float newworld)
	{
		if (World_Location.x == newworld)
			return false;
		World_Location.x = newworld;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetWorldLocationY(float newworld)
	{
		if (World_Location.y == newworld)
			return false;
		World_Location.y = newworld;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetWorldLocationZ(float newworld)
	{
		if (World_Location.z == newworld)
			return false;
		World_Location.z = newworld;
		TransformSystem::AddUpdateData(this);
		return true;

	}

	bool TransformComponent::SetWorldQuaternion(VPMath::Quaternion newworld)
	{
		if (World_Quaternion == newworld)
			return false;
		World_Quaternion = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}

	bool TransformComponent::SetWorldRotation(VPMath::Vector3 newworld)
	{
		if (World_Rotation == newworld)
			return false;
		World_Rotation = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}
	bool TransformComponent::SetWorldRotationX(float newworld)
	{										   
		if (World_Rotation.x == newworld)		   
			return false;							   
		World_Rotation.x = newworld;			   
		TransformSystem::AddUpdateData(this);  
		return true;
	}
	bool TransformComponent::SetWorldRotationY(float newworld)
	{										   
		if (World_Rotation.y == newworld)		   
			return false;							   
		World_Rotation.y = newworld;			   
		TransformSystem::AddUpdateData(this);  
		return true;
	}
	bool TransformComponent::SetWorldRotationZ(float newworld)
	{
		if (World_Rotation.z == newworld)
			return false;
		World_Rotation.z = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}
	bool TransformComponent::SetWorldScale(VPMath::Vector3 newworld)
	{
		if (World_Scale == newworld)
			return false;
		World_Scale = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}

	bool TransformComponent::SetWorldScaleX(float newworld)
	{
		if (World_Scale.x == newworld)
			return false;
		World_Scale.x = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}

	bool TransformComponent::SetWorldScaleY(float newworld)
	{
		if (World_Scale.y == newworld)
			return false;
		World_Scale.y = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}

	bool TransformComponent::SetWorldScaleZ(float newworld)
	{
		if (World_Scale.z == newworld)
			return false;
		World_Scale.z = newworld;
		TransformSystem::AddUpdateData(this);
		return true;
	}
