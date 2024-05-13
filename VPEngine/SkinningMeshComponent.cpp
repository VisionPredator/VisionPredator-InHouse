#include "pch.h"
#include "SkinningMeshComponent.h"
namespace VisPred
{
	META_REGISTRATION
	{
		entt::meta<SkinningMeshComponent>()
			.data<&SkinningMeshComponent::name>("name"_hs)
		.func<&SkinningMeshComponent::SerializeComponent>("SerializeComponent"_hs)
		.func<&SkinningMeshComponent::DeserializeComponent>("DeserializeComponent"_hs);
	}

		SkinningMeshComponent::SkinningMeshComponent()
	{
	}

}