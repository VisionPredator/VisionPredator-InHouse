#include "pch.h"
#include "DataRegister_Engine.h"
#include "Components.h"
#include "ParticleComponent.h"
#include "../VPGraphics/MeshFilter.h"
#include "../VPGraphics/CBuffer.h"

void EngineRegister::Register_Metadata()
{
	EngineRegister::Register_VPMath();
	EngineRegister::Register_Structs();
	EngineRegister::Register_EnumClass();
	EngineRegister::Register_Value();
	EngineRegister::Register_Components();

}

void EngineRegister::Register_EnumClass()
{
	META_ADD_ENUMCLASS(GeoMetryFilter, GeoMetryFilter::Axis, GeoMetryFilter::Box, GeoMetryFilter::Frustum, GeoMetryFilter::Grid);
	META_ADD_ENUMCLASS(LightType, LightType::Direction, LightType::Spot, LightType::Point);
	META_ADD_ENUMCLASS(PassState, PassState::ObjectMask);

}

void EngineRegister::Register_Structs()
{
}

void EngineRegister::Register_Value()
{
	META_ADD_VALUE(std::vector<std::string>);
	META_ADD_VALUE(std::vector<std::wstring>);
}

void EngineRegister::Register_VPMath()
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



void EngineRegister::Register_Components()
{
	META_ADD_COMP(IDComponent, IDComponent::Name);
	META_ADD_COMP(IdentityComponent, IdentityComponent::UUID);
	META_ADD_COMP(TransformComponent, TransformComponent::Local_Location, TransformComponent::Local_Rotation, TransformComponent::Local_Quaternion, TransformComponent::Local_Scale, TransformComponent::World_Location, TransformComponent::World_Rotation, TransformComponent::World_Quaternion, TransformComponent::World_Scale, TransformComponent::FrontVector, TransformComponent::RightVector);
	META_ADD_COMP(MeshComponent, MeshComponent::FBX,MeshComponent::LightMapOffset, MeshComponent::LightMapTiling,MeshComponent::LightMapScale, MeshComponent::LightMapIndex, MeshComponent::MaskColor);
	META_ADD_COMP(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_COMP(LightComponent, LightComponent::type, LightComponent::intensity, LightComponent::color, LightComponent::direction, LightComponent::attenuation, LightComponent::range, LightComponent::spot);
	META_ADD_COMP(Parent, Parent::ParentID);
	META_ADD_COMP(Children, Children::ChildrenID);
	META_ADD_COMP(CameraComponent, CameraComponent::IsMain, CameraComponent::NearZ, CameraComponent::FarZ, CameraComponent::FOV);
	META_ADD_COMP(AnimationComponent, AnimationComponent::FBX, AnimationComponent::duration, AnimationComponent::speed, AnimationComponent::transitionDuration,AnimationComponent::curAni,AnimationComponent::preAni,AnimationComponent::isLoop, AnimationComponent::isPlay);
	META_ADD_COMP(SkinningMeshComponent, SkinningMeshComponent::FBX);
	META_ADD_COMP(ParticleComponent, ParticleComponent::TexturePath, ParticleComponent::MaxParticle);
	META_ADD_COMP(GeometryComponent, GeometryComponent::FBXFilter, GeometryComponent::color, GeometryComponent::UseTexture, GeometryComponent::TextureName);
	META_ADD_COMP(RigidBodyComponent, RigidBodyComponent::IsDynamic, RigidBodyComponent::ColliderType, RigidBodyComponent::ColliderShape, RigidBodyComponent::BoxInfo, RigidBodyComponent::SphereInfo, RigidBodyComponent::CapsuleInfo, RigidBodyComponent::DefaultColliderInfo);
	META_ADD_COMP(ControllerComponent, ControllerComponent::Contollerinfo, ControllerComponent::CapsuleControllerinfo, ControllerComponent::Velocity, ControllerComponent::MaxSpeed, ControllerComponent::Acceleration, ControllerComponent::StaticFriction, ControllerComponent::DynamicFriction, ControllerComponent::JumpSpeed, ControllerComponent::JumpXZAcceleration, ControllerComponent::JumpXZDeceleration, ControllerComponent::GravityWeight);
	META_ADD_COMP(LifeTimeComponent, LifeTimeComponent::LifeTime);
	META_ADD_COMP(Sprite2DComponent, Sprite2DComponent::TexturePath, Sprite2DComponent::PosXPercent, Sprite2DComponent::PosYPercent, Sprite2DComponent::Scale, Sprite2DComponent::Layer, Sprite2DComponent::Color);
	META_ADD_COMP(NavAgentComponent, NavAgentComponent::TargetName, NavAgentComponent::IsChase);
	META_ADD_COMP(SocketComponent, SocketComponent::ConnectedEntity, SocketComponent::SocketName, SocketComponent::IsConnected, SocketComponent::Offset, SocketComponent::OffsetRotation);
}

