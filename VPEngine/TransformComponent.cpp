#include "pch.h"
#include "TransformComponent.h"

	META_REGISTRATION
	{
		META_ADD_MEMBER(Parent,Parent::ParentID)
		META_ADD_MEMBER(Children,Children::ChildrenID)
		META_ADD_MEMBER(TransformComponent,TransformComponent::Local_Location,TransformComponent::Local_Quaternion,TransformComponent::Local_Scale)





	}

	TransformComponent::TransformComponent()
	{
	}

