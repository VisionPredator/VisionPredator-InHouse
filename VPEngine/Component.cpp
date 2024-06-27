#include "pch.h"
#include "Component.h"
#include "Components.h"
META_REGISTRATION
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name)

	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID)
	META_ADD_MEMBER(TransformComponent
		, TransformComponent::Local_Location
		, TransformComponent::Local_Rotation
		, TransformComponent::Local_Quaternion
		, TransformComponent::Local_Scale)
	META_ADD_MEMBER(MeshComponent
		, MeshComponent::FBX)
	META_ADD_MEMBER(PlayerComponent
		,PlayerComponent::HP)
	META_ADD_MEMBER(SkinningMeshComponent
		,SkinningMeshComponent::FBX)
	META_ADD_MEMBER(Parent
		, Parent::ParentID)
	META_ADD_MEMBER(Children
		, Children::ChildrenID)
	META_ADD_MEMBER(CameraComponent,CameraComponent::NearZ
		,CameraComponent::FarZ
		,CameraComponent::FOV
		,CameraComponent::Ratio)
}



	Component::Component()
	{
	}
