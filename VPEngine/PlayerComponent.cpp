#include "pch.h"
#include "PlayerComponent.h"
META_REGISTRATION
{
	META_ADD_MEMBER(PlayerComponent,PlayerComponent::HP)
}
IMPLEMENT_COMPONENT_TYPE(PlayerComponent);

PlayerComponent::PlayerComponent()
{
}
