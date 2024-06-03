#include "pch.h"
#include "IDComponent.h"

META_REGISTRATION
{
	META_ADD_MEMBER(IDComponent,IDComponent::Name);
}

IMPLEMENT_COMPONENT_TYPE(IDComponent);

IDComponent::IDComponent()
{
}
