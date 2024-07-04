#include "pch.h"
#include "DataRegister.h"
#include "Components.h"
#include "../VPGraphics/MeshFilter.h"
#include "../VPGraphics/CBuffer.h"

void VPRegister::Register_EnumClass()
{
	entt::meta<MeshFilter>()
		.prop(Reflection::Prop::Name, "MeshFilter")
		.conv<std::underlying_type_t<MeshFilter>>()
		.data<MeshFilter::Axis>("Axis"_hs)
		.prop(Reflection::Prop::Name, "Axis")
		.data<MeshFilter::Box>("Box"_hs)
		.prop(Reflection::Prop::Name, "Box")
		.data<MeshFilter::Grid>("Grid"_hs)
		.prop(Reflection::Prop::Name, "Grid")
		.data<MeshFilter::Static>("Static"_hs)
		.prop(Reflection::Prop::Name, "Static")
		.data<MeshFilter::Circle>("Circle"_hs)
		.prop(Reflection::Prop::Name, "Circle");

	entt::meta<LightType>()
		.prop(Reflection::Prop::Name, "LightType")
		.conv<std::underlying_type_t<LightType>>()
		.data<LightType::Direction>("Direction"_hs)
		.prop(Reflection::Prop::Name, "Direction")
		.data<LightType::Spot>("Spot"_hs)
		.prop(Reflection::Prop::Name, "Spot")
		.data<LightType::Point>("Point"_hs)
		.prop(Reflection::Prop::Name, "Point");
}

void VPRegister::Register_Value()
{
	entt::meta<std::vector<std::string>>()
		.type("std::vector<std::string>"_hs);
	entt::meta<std::vector<std::wstring>>()
		.type("std::vector<std::wstring>"_hs);
}

NLOHMANN_JSON_SERIALIZE_ENUM(MeshFilter, {
{MeshFilter::Axis, "Axis"},
{MeshFilter::Box, "Box"},
{MeshFilter::Grid, "Grid"},
{MeshFilter::Static, "Static"},
{MeshFilter::Skinning, "Skinning"},
{MeshFilter::Circle, "Circle"},
{MeshFilter::None, "None"}	});
NLOHMANN_JSON_SERIALIZE_ENUM(LightType, {
{LightType::Direction, "Direction"},
{LightType::Spot, "Spot"},
{LightType::Point, "Point"},
{LightType::End, "End"}	});



void VPRegister::Register_Metadata()
{
	VPRegister::Register_Components();
	VPRegister::Register_EnumClass();
	VPRegister::Register_Value();
}

void VPRegister::Register_Components()
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name);
	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID);
	META_ADD_MEMBER(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale);
	META_ADD_MEMBER(MeshComponent, MeshComponent::FBXFilter, MeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(LightComponent, LightComponent::type, LightComponent::intensity, LightComponent::color, LightComponent::direction, LightComponent::attenuation,LightComponent::testVector,LightComponent::testwVector, LightComponent::range, LightComponent::spot);
	META_ADD_MEMBER(Parent, Parent::ParentID);
	META_ADD_MEMBER(Children, Children::ChildrenID);
	META_ADD_MEMBER(CameraComponent,CameraComponent::IsMain, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV, CameraComponent::Ratio);
	META_ADD_MEMBER(AnimationComponent, AnimationComponent::curAnimation, AnimationComponent::duration, AnimationComponent::animeName1)
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX)
	META_ADD_MEMBER(RigidBodyComponent, RigidBodyComponent::Test)
}

