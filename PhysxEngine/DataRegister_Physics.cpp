#include "CommonInclude.h"
#include "DataRegister_Physics.h"
#include "DynamicRigidBody.h"
#include "StaticRigidBody.h"
#include "CapsuleController.h"
#include "Controller.h"
#include "RigidBody.h"
#include "DataRegister_Physics.h"
void PhysicsRegister::Register_Metadata()
{
	static bool DoOnce = false;
	if (!DoOnce)
	{
		PhysicsRegister::Register_VPMath();
		PhysicsRegister::Register_Structs();
		PhysicsRegister::Register_EnumClass();
		PhysicsRegister::Register_Value();
		PhysicsRegister::Register_Components();
		DoOnce = true;
	}
}

void PhysicsRegister::Register_Components()
{
}

void PhysicsRegister::Register_EnumClass()
{
	META_ADD_ENUMCLASS(EColliderType, EColliderType::TRIGGER, EColliderType::COLLISION);
	META_ADD_ENUMCLASS(EPhysicsLayer, EPhysicsLayer::GROUND, EPhysicsLayer::WALL, EPhysicsLayer::TOP, EPhysicsLayer::PLAYER, EPhysicsLayer::OBJECT, EPhysicsLayer::ENEMY, EPhysicsLayer::DOOR, EPhysicsLayer::ACTIVEDOOR, EPhysicsLayer::TRIGGER);
	META_ADD_ENUMCLASS(ControllerPivot, ControllerPivot::CENTER, ControllerPivot::END, ControllerPivot::FOOT);
	META_ADD_ENUMCLASS(EColliderShape, EColliderShape::BOX, EColliderShape::SPHERE, EColliderShape::CAPSULE, EColliderShape::CONVEX);
}

void PhysicsRegister::Register_Structs()
{
	META_ADD_STRUCT(PhysicsInfo, PhysicsInfo::Gravity, PhysicsInfo::FrameRate, PhysicsInfo::CollisionMatrix);
	META_ADD_STRUCT(ColliderInfo, ColliderInfo::PhysicsLayer, ColliderInfo::OffSet, ColliderInfo::LinearLock, ColliderInfo::AngleLock, ColliderInfo::StaticFriction, ColliderInfo::DynamicFriction, ColliderInfo::Restitution, ColliderInfo::Density);
	META_ADD_STRUCT(BoxColliderInfo, BoxColliderInfo::colliderInfo, BoxColliderInfo::Extent);
	META_ADD_STRUCT(SphereColliderInfo, SphereColliderInfo::colliderInfo, SphereColliderInfo::Radius);
	META_ADD_STRUCT(CapsuleColliderInfo, CapsuleColliderInfo::colliderInfo, CapsuleColliderInfo::Radius, CapsuleColliderInfo::HalfHeight);
	META_ADD_STRUCT(CapsuleControllerInfo, CapsuleControllerInfo::contactOffset, CapsuleControllerInfo::LocalOffset, CapsuleControllerInfo::height, CapsuleControllerInfo::position, CapsuleControllerInfo::radius, CapsuleControllerInfo::slopeLimit, CapsuleControllerInfo::stepOffset);
	entt::meta<ControllerInfo>().type("ControllerInfo"_hs).data<&ControllerInfo::LayerNumber >("ControllerInfo::LayerNumber"_hs).prop(Reflection::Prop::Name, "ControllerInfo::LayerNumber").data<&ControllerInfo::Pivot >("ControllerInfo::Pivot"_hs).prop(Reflection::Prop::Name, "ControllerInfo::Pivot");
}

void PhysicsRegister::Register_Value()
{
	META_ADD_VALUE(DynamicRigidBody);
	META_ADD_VALUE(StaticRigidBody);
	META_ADD_VALUE(CapsuleController);
	META_ADD_VALUE(Controller);
	META_ADD_VALUE(RigidBody);
	entt::meta<std::array<int, (int)EPhysicsLayer::END>>().type("std::array<int,(int)EPhysicsLayer::END>"_hs);
	entt::meta<std::array<bool, 3>>().type("std::array<bool, 3>"_hs);
}

void PhysicsRegister::Register_VPMath()
{
}
