#include "pch.h"
#include "Component.h"
#include "Components.h"
META_REGISTRATION
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name)

	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID)
	entt::meta<TransformComponent>()
	.base<Component>()
	.type("TransformComponent"_hs)
	.data<&TransformComponent::Local_Location >("TransformComponent::Local_Location"_hs)
	.prop(Reflection::Prop::Name, "TransformComponent::Local_Location")
	.data<&TransformComponent::Local_Rotation >("TransformComponent::Local_Rotation"_hs)
	.prop(Reflection::Prop::Name, "TransformComponent::Local_Rotation").data<&TransformComponent::Local_Quaternion >("TransformComponent::Local_Quaternion"_hs).prop(Reflection::Prop::Name, "TransformComponent::Local_Quaternion").data<&TransformComponent::Local_Scale >("TransformComponent::Local_Scale"_hs).prop(Reflection::Prop::Name, "TransformComponent::Local_Scale").func<&TransformComponent::SerializeComponent>("SerializeComponent"_hs).func<&TransformComponent::DeserializeComponent>("DeserializeComponent"_hs).func<&TransformComponent::AddComponent>("AddComponent"_hs);





	META_ADD_MEMBER(MeshComponent, MeshComponent::name)
	META_ADD_MEMBER(PlayerComponent,PlayerComponent::HP)
	META_ADD_MEMBER(SkinningMeshComponent,SkinningMeshComponent::Name)
	META_ADD_MEMBER(Parent, Parent::ParentID)
	META_ADD_MEMBER(Children, Children::ChildrenID)
	META_ADD_MEMBER(CameraComponent,CameraComponent::NearZ,CameraComponent::FarZ,CameraComponent::FOV,CameraComponent::Ratio)
}



	Component::Component()
	{
	}
