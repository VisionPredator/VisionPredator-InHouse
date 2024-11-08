#include "pch.h"
#include "RigidBody.h"


RigidBody::RigidBody(VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo) : m_ColliderType{ type }
, m_UserData{ info.colliderInfo.EntityID,false }
, m_LayerNum{ info.colliderInfo.PhysicsLayer }
{
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	SetColliderinfo(info);
}

RigidBody::RigidBody(VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo) : m_ColliderType{ type }
, m_UserData{ info.colliderInfo.EntityID,false }
, m_LayerNum{ info.colliderInfo.PhysicsLayer }
{
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	SetColliderinfo(info);
}

RigidBody::RigidBody(VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const  VPPhysics::PhysicsInfo& engininfo) : m_ColliderType{ type }
, m_UserData{ info.colliderInfo.EntityID,false }
, m_LayerNum{ info.colliderInfo.PhysicsLayer }
{
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	SetColliderinfo(info);
}

RigidBody::RigidBody(VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo) : m_ColliderType{ type }
, m_UserData{ info.colliderInfo.EntityID,false }
, m_LayerNum{ info.colliderInfo.PhysicsLayer }
{
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	SetColliderinfo(info);
}

RigidBody::~RigidBody()
{
}
