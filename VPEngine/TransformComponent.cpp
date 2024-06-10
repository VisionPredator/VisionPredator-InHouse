#include "pch.h"
#include "TransformComponent.h"

META_REGISTRATION
{
	META_ADD_MEMBER(Parent,Parent::ParentID)
	META_ADD_MEMBER(Children,Children::ChildrenID)
	META_ADD_MEMBER(TransformComponent,TransformComponent::Local_Location, TransformComponent::Local_Quaternion, TransformComponent::LocalTransform)







	}
	IMPLEMENT_COMPONENT_TYPE(Parent);
	IMPLEMENT_COMPONENT_TYPE(Children);
	IMPLEMENT_COMPONENT_TYPE(TransformComponent);


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
