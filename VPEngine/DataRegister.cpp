#include "pch.h"
#include "DataRegister.h"
#include "Components.h"

void VPRegister::Register_EnumClass()
{

	entt::meta<TestEnum>()
		.prop(entt::hashed_string("name"), "TestEnum")
		.conv<std::underlying_type_t<TestEnum>>()
		.data<TestEnum::a>("a"_hs)
		.prop(entt::hashed_string("name"), "a")
		.data<TestEnum::b>("b"_hs)
		.prop(entt::hashed_string("name"), "b")
		.data<TestEnum::c>("c"_hs)
		.prop(entt::hashed_string("name"), "c")
		.data<TestEnum::d>("d"_hs)
		.prop(entt::hashed_string("name"), "d");



}



void VPRegister::Register_Components()
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name);
	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID);
	META_ADD_MEMBER(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale);
	META_ADD_MEMBER(MeshComponent, MeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::Name, SkinningMeshComponent::Test);
	META_ADD_MEMBER(Parent, Parent::ParentID);
	META_ADD_MEMBER(Children, Children::ChildrenID);
	META_ADD_MEMBER(CameraComponent, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV, CameraComponent::Ratio);
}

