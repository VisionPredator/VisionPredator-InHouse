#include "pch.h"
#include "SkinningMeshComponent.h"
	META_REGISTRATION
	{
		entt::meta<SkinningMeshComponent>()
			.data<&SkinningMeshComponent::Name>("name"_hs)
		.func<&SkinningMeshComponent::SerializeComponent>("SerializeComponent"_hs)
		.func<&SkinningMeshComponent::DeserializeComponent>("DeserializeComponent"_hs);
	}

		SkinningMeshComponent::SkinningMeshComponent()
	{
	}

