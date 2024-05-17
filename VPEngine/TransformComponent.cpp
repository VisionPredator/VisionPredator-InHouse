#include "pch.h"
#include "TransformComponent.h"

	META_REGISTRATION
	{
		META_ADD_MEMBER(Parent,Parent::ParentID)
		META_ADD_MEMBER(Children,Children::ChildrenID)
		META_ADD_MEMBER(TransformComponent,TransformComponent::Local_Location,TransformComponent::Local_Quaternion,TransformComponent::Local_Scale)

	}
	IMPLEMENT_COMPONENT_TYPE(Parent);
	IMPLEMENT_COMPONENT_TYPE(Children);
	IMPLEMENT_COMPONENT_TYPE(TransformComponent);


	TransformComponent::TransformComponent()
	{
	}

	Parent::Parent()
	{
	}

	Children::Children()
	{
	}
