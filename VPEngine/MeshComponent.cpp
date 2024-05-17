#include "pch.h"
#include "MeshComponent.h"
	META_REGISTRATION
	{
		entt::meta<MeshComponent>()
			.data<&MeshComponent::name>("name"_hs)
		.func<&MeshComponent::SerializeComponent>("SerializeComponent"_hs)
		.func<&MeshComponent::DeserializeComponent>("DeserializeComponent"_hs);
	}
	IMPLEMENT_COMPONENT_TYPE(MeshComponent);

		MeshComponent::MeshComponent()
	{
	}

