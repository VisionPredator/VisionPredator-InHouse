#include "pch.h"
#include "IdentityComponent.h"

namespace VisPred
{
	META_REGISTRATION
	{
		entt::meta<IdentityComponent>()
			.data<&IdentityComponent::UUID>("UUID"_hs)
		.func<&IdentityComponent::SerializeComponent>("SerializeComponent"_hs)
		.func<&IdentityComponent::DeserializeComponent>("DeserializeComponent"_hs);
		}

		IdentityComponent::IdentityComponent(		)
	{
	}

}