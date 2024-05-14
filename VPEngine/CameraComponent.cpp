#include "pch.h"
#include "CameraComponent.h"


META_REGISTRATION
{
	entt::meta<CameraComponent>()
		.data<&CameraComponent::testMatrix>("testMatrix"_hs)
		.func<&CameraComponent::SerializeComponent>("SerializeComponent"_hs)
		.func<&CameraComponent::DeserializeComponent>("DeserializeComponent"_hs);
}

CameraComponent::CameraComponent()
{
}
