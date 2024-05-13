#include "pch.h"
#include "IDComponent.h"

namespace VisPred
{
	META_REGISTRATION
	{
		entt::meta<IDComponent>()
			.data<&IDComponent::Name>("Name"_hs)
			.func<&IDComponent::SerializeComponent>("SerializeComponent"_hs)
			.func<&IDComponent::DeserializeComponent>("DeserializeComponent"_hs);
	}

	IDComponent::IDComponent()
	{
	}
}