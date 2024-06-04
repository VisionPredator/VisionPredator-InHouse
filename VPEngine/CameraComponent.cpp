#include "pch.h"
#include "CameraComponent.h"


META_REGISTRATION
{
	META_ADD_MEMBER(CameraComponent,CameraComponent::NearZ,CameraComponent::FarZ,CameraComponent::FOV,CameraComponent::Ratio);
}
IMPLEMENT_COMPONENT_TYPE(CameraComponent);

CameraComponent::CameraComponent()
{
}
