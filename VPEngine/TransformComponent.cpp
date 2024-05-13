#include "pch.h"
#include "TransformComponent.h"

namespace VisPred
{
	META_REGISTRATION
	{
		entt::meta<Parent>()
			.data<&Parent::ParentID>("ParentID"_hs)
			.func<&Parent::SerializeComponent>("SerializeComponent"_hs)
			.func<&Parent::DeserializeComponent>("DeserializeComponent"_hs);
		entt::meta<Children>()
			.data<&Children::ChildrenID>("ChildrenID"_hs)
			.func<&Children::SerializeComponent>("SerializeComponent"_hs)
			.func<&Children::DeserializeComponent>("DeserializeComponent"_hs);

		entt::meta<TransformComponent>()
			.data<&TransformComponent::Local_Location>("Local_Location"_hs)
			.data<&TransformComponent::Local_Quaternion>("Local_Quaternion"_hs)
			.data<&TransformComponent::Local_Scale>("Local_Scale"_hs)
			.func<&TransformComponent::SerializeComponent>("SerializeComponent"_hs)
			.func<&TransformComponent::DeserializeComponent>("DeserializeComponent"_hs);
	}

		VisPred::TransformComponent::TransformComponent()
	{
	}

}