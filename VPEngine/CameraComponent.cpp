#include "pch.h"
#include "CameraComponent.h"


META_REGISTRATION
{
	META_ADD_MEMBER(CameraComponent,CameraComponent::testMatrix);
}
IMPLEMENT_COMPONENT_TYPE(CameraComponent);
CameraComponent::CameraComponent()
{
}
