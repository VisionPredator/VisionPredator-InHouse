#include "pch.h"
#include "DataRegister.h"
#include "Components.h"
#include "ParticleComponent.h"
#include "../VPGraphics/MeshFilter.h"
#include "../VPGraphics/CBuffer.h"
#include "../PhysxEngine/VPPhysicsStructs.h"
#include "../PhysxEngine/DynamicRigidBody.h"
#include "../PhysxEngine/StaticRigidBody.h"
using namespace VPPhysics;

void VPRegister::Register_Metadata()
{
	VPRegister::Register_VPMath();
	VPRegister::Register_Structs();
	VPRegister::Register_EnumClass();
	VPRegister::Register_Value();
	VPRegister::Register_Components();

}

void VPRegister::Register_EnumClass()
{
	META_ADD_ENUMCLASS(MeshFilter, MeshFilter::Axis, MeshFilter::Box, MeshFilter::Grid, MeshFilter::Static, MeshFilter::Circle);
	META_ADD_ENUMCLASS(LightType, LightType::Direction, LightType::Spot, LightType::Point);
	META_ADD_ENUMCLASS(EColliderType, EColliderType::TRIGGER, EColliderType::COLLISION);
	entt::meta<EPhysicsLayer>().prop(Reflection::Prop::Name, "EPhysicsLayer").conv<std::underlying_type_t<EPhysicsLayer>>().data<EPhysicsLayer::GROUND >("EPhysicsLayer::GROUND"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::GROUND").data<EPhysicsLayer::WALL >("EPhysicsLayer::WALL"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::WALL").data<EPhysicsLayer::TOP >("EPhysicsLayer::TOP"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::TOP").data<EPhysicsLayer::PLAYER >("EPhysicsLayer::PLAYER"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::PLAYER").data<EPhysicsLayer::OBJECT >("EPhysicsLayer::OBJECT"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::OBJECT").data<EPhysicsLayer::ENEMY >("EPhysicsLayer::ENEMY"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::ENEMY").data<EPhysicsLayer::DOOR >("EPhysicsLayer::DOOR"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::DOOR").data<EPhysicsLayer::ACTIVEDOOR >("EPhysicsLayer::ACTIVEDOOR"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::ACTIVEDOOR").data<EPhysicsLayer::TRIGGER >("EPhysicsLayer::TRIGGER"_hs).prop(Reflection::Prop::Name, "EPhysicsLayer::TRIGGER");
	META_ADD_ENUMCLASS(EColliderShape, EColliderShape::BOX, EColliderShape::SPHERE, EColliderShape::CAPSULE);

}

void VPRegister::Register_Structs()
{
	META_ADD_STRUCT(PhysicsInfo, PhysicsInfo::Gravity, PhysicsInfo::FrameRate, PhysicsInfo::CollisionMatrix);
	META_ADD_STRUCT(ColliderInfo, ColliderInfo::PhysicsLayer, ColliderInfo::OffSet, ColliderInfo::LinearLock, ColliderInfo::AngleLock, ColliderInfo::StaticFriction, ColliderInfo::DynamicFriction, ColliderInfo::Restitution, ColliderInfo::Density);
	META_ADD_STRUCT(BoxColliderInfo, BoxColliderInfo::colliderInfo, BoxColliderInfo::UseAABB, BoxColliderInfo::Extent);
	META_ADD_STRUCT(SphereColliderInfo, SphereColliderInfo::colliderInfo, SphereColliderInfo::Radius);
	META_ADD_STRUCT(CapsuleColliderInfo, CapsuleColliderInfo::colliderInfo, CapsuleColliderInfo::Radius, CapsuleColliderInfo::HalfHeight);
}

void VPRegister::Register_Value()
{
	META_ADD_VALUE(std::vector<std::string>);
	META_ADD_VALUE(DynamicRigidBody);
	META_ADD_VALUE(StaticRigidBody);
	META_ADD_VALUE(RigidBody);
	META_ADD_VALUE(std::vector<std::wstring>);
	entt::meta<std::array<int, (int)EPhysicsLayer::END>>().type("std::array<int,(int)EPhysicsLayer::END>"_hs);
	entt::meta<std::array<bool, 3>>().type("std::array<bool, 3>"_hs);

}

void VPRegister::Register_VPMath()
{
	using namespace VPMath;
	entt::meta<Vector2>()
		.data<&Vector2::x>("x"_hs)
		.data<&Vector2::y>("y"_hs);

	entt::meta<Vector3>()
		.data<&Vector3::x>("x"_hs)
		.data<&Vector3::y>("y"_hs)
		.data<&Vector3::z>("z"_hs);

	entt::meta<Vector4>()
		.data<&Vector4::x>("x"_hs)
		.data<&Vector4::y>("y"_hs)
		.data<&Vector4::z>("z"_hs)
		.data<&Vector4::w>("w"_hs);

	entt::meta<Vector4>()
		.data<&Vector4::x>("x"_hs)
		.data<&Vector4::y>("y"_hs)
		.data<&Vector4::z>("z"_hs)
		.data<&Vector4::w>("w"_hs);
	entt::meta<Matrix>()
		.data<&Matrix::_11>("_11"_hs).data<&Matrix::_12>("_12"_hs).data<&Matrix::_13>("_13"_hs).data<&Matrix::_14>("_14"_hs)
		.data<&Matrix::_21>("_21"_hs).data<&Matrix::_22>("_22"_hs).data<&Matrix::_23>("_23"_hs).data<&Matrix::_24>("_24"_hs)
		.data<&Matrix::_31>("_31"_hs).data<&Matrix::_32>("_32"_hs).data<&Matrix::_33>("_33"_hs).data<&Matrix::_34>("_34"_hs)
		.data<&Matrix::_41>("_41"_hs).data<&Matrix::_42>("_42"_hs).data<&Matrix::_43>("_43"_hs).data<&Matrix::_44>("_44"_hs);

	entt::meta<Plane>()
		.data<&Plane::x>("x"_hs)
		.data<&Plane::y>("y"_hs)
		.data<&Plane::z>("z"_hs)
		.data<&Plane::w>("w"_hs);

	entt::meta<Quaternion>()
		.data<&Quaternion::x>("x"_hs)
		.data<&Quaternion::y>("y"_hs)
		.data<&Quaternion::z>("z"_hs)
		.data<&Quaternion::w>("w"_hs);

	entt::meta<Color>()
		.data<&Color::x>("x"_hs)
		.data<&Color::y>("y"_hs)
		.data<&Color::z>("z"_hs)
		.data<&Color::w>("w"_hs);

	entt::meta<Ray>()
		.data<&Ray::position>("position"_hs)
		.data<&Ray::direction>("direction"_hs);

	entt::meta<Viewport>()
		.data<&Viewport::x>("x"_hs)
		.data<&Viewport::y>("y"_hs)
		.data<&Viewport::width>("width"_hs)
		.data<&Viewport::height>("height"_hs)
		.data<&Viewport::minDepth>("minDepth"_hs)
		.data<&Viewport::maxDepth>("maxDepth"_hs);
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



void VPRegister::Register_Components()
{
	META_ADD_MEMBER(IDComponent, IDComponent::Name);
	META_ADD_MEMBER(IdentityComponent, IdentityComponent::UUID);
	META_ADD_MEMBER(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale, TransformComponent::World_Location, TransformComponent::World_Rotation,TransformComponent::World_Quaternion, TransformComponent::World_Scale);
	META_ADD_MEMBER(MeshComponent, MeshComponent::FBXFilter, MeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(LightComponent, LightComponent::type, LightComponent::intensity, LightComponent::color, LightComponent::direction, LightComponent::attenuation, LightComponent::range, LightComponent::spot);
	META_ADD_MEMBER(Parent, Parent::ParentID);
	META_ADD_MEMBER(Children, Children::ChildrenID);
	META_ADD_MEMBER(CameraComponent,CameraComponent::IsMain, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV, CameraComponent::Ratio);
	META_ADD_MEMBER(AnimationComponent, AnimationComponent::curAnimation, AnimationComponent::duration, AnimationComponent::speed, AnimationComponent::animationList);
	META_ADD_MEMBER(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_MEMBER(PlayerComponent, PlayerComponent::HP, PlayerComponent::Maxspeed, PlayerComponent::Speed, PlayerComponent::Accel,PlayerComponent::Maxspeed, PlayerComponent::JumpFoce, PlayerComponent::MaxJumpCount, PlayerComponent::Jumpcount, PlayerComponent::Isground);
	META_ADD_MEMBER(ParticleComponent, ParticleComponent::TexturePath, ParticleComponent::MaxParticle);
	META_ADD_MEMBER(RigidBodyComponent, RigidBodyComponent::IsDynamic, RigidBodyComponent::ColliderType, RigidBodyComponent::ColliderShape, RigidBodyComponent::BoxInfo, RigidBodyComponent::SphereInfo, RigidBodyComponent::CapsuleInfo, RigidBodyComponent::ColliderInfo);
}

