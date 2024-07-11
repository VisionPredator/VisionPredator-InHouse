#include "pch.h"
#include "DataRegister.h"
#include "Components.h"
#include "../VPGraphics/MeshFilter.h"
#include "../VPGraphics/CBuffer.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
using namespace VPPhysics;


void VPRegister::Register_Metadata()
{
	VPRegister::Register_EnumClass();
	VPRegister::Register_Structs();
	VPRegister::Register_Value();
	VPRegister::Register_Components();
}
struct Example {
	std::array<int, (int)EPhysicsLayer::END> array;
};

// 메타 데이터 등록 함수
void registerMeta() {
	// std::array<int, 16> 타입 등록
	entt::meta<std::array<int, (int)EPhysicsLayer::END>>()
		.type("std::array<int, (int)EPhysicsLayer::END>"_hs)
		.prop("description"_hs, "A fixed-size array of 16 integers");

	// Example 타입 등록
	entt::meta<Example>()
		.type("Example"_hs)
		.data<&Example::array>("array"_hs)
		.prop("description"_hs, "An array of 16 integers");
}
void VPRegister::Register_EnumClass()
{
	META_ADD_ENUMCLASS(MeshFilter, MeshFilter::Axis, MeshFilter::Box, MeshFilter::Grid, MeshFilter::Static, MeshFilter::Circle);
	META_ADD_ENUMCLASS(LightType, LightType::Direction, LightType::Spot, LightType::Point);
	META_ADD_ENUMCLASS(EColliderType, EColliderType::TRIGGER, EColliderType::COLLISION);
	META_ADD_ENUMCLASS(EPhysicsLayer, EPhysicsLayer::GROUND, EPhysicsLayer::WALL, EPhysicsLayer::TOP, EPhysicsLayer::PLAYER, EPhysicsLayer::OBJECT, EPhysicsLayer::ENEMY, EPhysicsLayer::DOOR, EPhysicsLayer::ACTIVEDOOR, EPhysicsLayer::TRIGGER);
	META_ADD_ENUMCLASS(EColliderShape, EColliderShape::BOX, EColliderShape::SPHERE, EColliderShape::CAPSULE);
}

void VPRegister::Register_Structs()
{
	META_ADD_STRUCT(PhysicsInfo, PhysicsInfo::Gravity, PhysicsInfo::CollisionMatrix);
	META_ADD_STRUCT(ColliderInfo, ColliderInfo::PhysicsLayer, ColliderInfo::StaticFriction, ColliderInfo::DynamicFriction, ColliderInfo::Restitution, ColliderInfo::Density);
	META_ADD_STRUCT(BoxColliderInfo, BoxColliderInfo::colliderInfo, BoxColliderInfo::Extent);
	META_ADD_STRUCT(SphereColliderInfo, SphereColliderInfo::colliderInfo, SphereColliderInfo::Radius);
	META_ADD_STRUCT(CapsuleColliderInfo, CapsuleColliderInfo::colliderInfo, CapsuleColliderInfo::Radius, CapsuleColliderInfo::HalfHeight);
}

void VPRegister::Register_Value()
{
	META_ADD_VALUE(std::vector<std::string>);
	META_ADD_VALUE(std::vector<std::wstring>);
	entt::meta<std::array<int, (int)EPhysicsLayer::END>>().type("std::array<int,(int)EPhysicsLayer::END>"_hs);
}

void VPRegister::Register_Components()
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name);
	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID);
	META_ADD_MEMBER(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale);
	META_ADD_MEMBER(MeshComponent, MeshComponent::FBXFilter, MeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(LightComponent, LightComponent::type, LightComponent::intensity, LightComponent::color, LightComponent::direction, LightComponent::attenuation, LightComponent::testVector, LightComponent::testwVector, LightComponent::range, LightComponent::spot);
	META_ADD_MEMBER(Parent, Parent::ParentID);
	META_ADD_MEMBER(Children, Children::ChildrenID);
	META_ADD_MEMBER(CameraComponent, CameraComponent::IsMain, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV, CameraComponent::Ratio);
	META_ADD_MEMBER(AnimationComponent, AnimationComponent::curAnimation, AnimationComponent::duration, AnimationComponent::animeName1);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(RigidBodyComponent, RigidBodyComponent::IsDynamic, RigidBodyComponent::ColliderType, RigidBodyComponent::ColliderShape,  RigidBodyComponent::BoxInfo, RigidBodyComponent::SphereInfo, RigidBodyComponent::CapsuleInfo, RigidBodyComponent::ColliderInfo);
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
{LightType::End, "End"} });
NLOHMANN_JSON_SERIALIZE_ENUM(VPPhysics::EColliderType, {
{VPPhysics::EColliderType::TRIGGER,"TRIGGER"},
{VPPhysics::EColliderType::COLLISION,"COLLISION"} });
NLOHMANN_JSON_SERIALIZE_ENUM(VPPhysics::EPhysicsLayer, {
{VPPhysics::EPhysicsLayer::GROUND,"GROUND" },
{VPPhysics::EPhysicsLayer::WALL,"WALL" },
{VPPhysics::EPhysicsLayer::TOP, "TOP" },
{VPPhysics::EPhysicsLayer::PLAYER,"PLAYER" },
{VPPhysics::EPhysicsLayer::OBJECT,"OBJECT" },
{VPPhysics::EPhysicsLayer::ENEMY,"ENEMY" },
{VPPhysics::EPhysicsLayer::DOOR,"DOOR" },
{VPPhysics::EPhysicsLayer::ACTIVEDOOR,"ACTIVEDOOR"},
{VPPhysics::EPhysicsLayer::TRIGGER ,"TRIGGER"} });
NLOHMANN_JSON_SERIALIZE_ENUM(VPPhysics::EColliderShape, {
{VPPhysics::EColliderShape::BOX,"BOX" },
{VPPhysics::EColliderShape::SPHERE,"SPHERE" },
{VPPhysics::EColliderShape::CAPSULE, "CAPSULE" } });





