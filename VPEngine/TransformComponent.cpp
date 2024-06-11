#include "pch.h"
#include "TransformComponent.h"




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
