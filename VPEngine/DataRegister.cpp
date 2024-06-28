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
		.data<MeshFilter::Skinning>("Skinning"_hs)
		.prop(Reflection::Prop::Name, "Skinning")
		.data<MeshFilter::Circle>("Circle"_hs)
		.prop(Reflection::Prop::Name, "Circle")
		.data<MeshFilter::None>("None"_hs)
		.prop(Reflection::Prop::Name, "None");

	entt::meta<LightType>()
		.prop(Reflection::Prop::Name, "LightType")
		.conv<std::underlying_type_t<LightType>>()
		.data<LightType::Direction>("Direction"_hs)
		.prop(Reflection::Prop::Name, "Direction")
		.data<LightType::Spot>("Spot"_hs)
		.prop(Reflection::Prop::Name, "Spot")
		.data<LightType::Point>("Point"_hs)
		.prop(Reflection::Prop::Name, "Point")
		.data<LightType::End>("End"_hs)
		.prop(Reflection::Prop::Name, "End");
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



void VPRegister::Register_Components()
{

	META_ADD_MEMBER(IDComponent, IDComponent::Name);
	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID);
	META_ADD_MEMBER(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale);
	META_ADD_MEMBER(MeshComponent, MeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(LightComponent, LightComponent::intensity, LightComponent::color, LightComponent::direction, LightComponent::attenuation, LightComponent::range, LightComponent::spot);
	META_ADD_MEMBER(Parent, Parent::ParentID);
	META_ADD_MEMBER(Children, Children::ChildrenID);
	META_ADD_MEMBER(CameraComponent, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV, CameraComponent::Ratio);
}
