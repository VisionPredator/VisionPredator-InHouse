#pragma once
#include "VPPhysicsStructs.h"
class RigidBody
{
public:
	RigidBody( VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const  VPPhysics::PhysicsInfo& engininfo);
	RigidBody( VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const  VPPhysics::PhysicsInfo& engininfo);
	RigidBody( VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
	RigidBody( VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
	virtual ~RigidBody(); // Make the destructor virtual

	inline const uint32_t GetID() const;
	VPPhysics::USERDATA m_UserData{};
	VPPhysics::EColliderType m_ColliderType{};
	VPMath::Vector3 mScale{1,1,1};
	VPPhysics::EPhysicsLayer m_LayerNum{};

	float m_Radius = 1.f;
	float m_HalfHeight = 1.f;
	physx::PxVec3 m_Extent{1,1,1};
	void SetRadius(float radius) { m_Radius= radius; }
	void SetHalfHeight(float halfheight) { m_HalfHeight= halfheight; }
	void SetExtent(physx::PxVec3 extent) { m_Extent= extent; }
	void SetExtent(VPMath::Vector3 extent) { m_Extent = { extent.x,extent.y,extent.z }; }
	void SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo);
	void SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo);
	void SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo);
	void SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo);

	float GetRadius() { return m_Radius; }
	float GetHalfHeight() { return m_HalfHeight; }
	physx::PxVec3 GetExtent() { return m_Extent; }
	virtual entt::id_type GetTypeID() const {
		return Reflection::GetTypeID<RigidBody>();
	}
	std::shared_ptr<VPPhysics::CapsuleColliderInfo> m_Capulseinfo;
	std::shared_ptr<VPPhysics::BoxColliderInfo> m_Boxinfo;
	std::shared_ptr<VPPhysics::ConvexColliderInfo> m_Convexinfo;
	std::shared_ptr<VPPhysics::SphereColliderInfo> m_Sphereinfo;
	std::shared_ptr<VPPhysics::PhysicsInfo> m_PhysicInfo;

};



const unsigned int RigidBody::GetID() const
{
	return m_UserData.entityID;
}

inline void RigidBody::SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo)
{
	m_Capulseinfo.reset();
	m_Boxinfo.reset();
	m_Convexinfo.reset();
	m_Sphereinfo.reset();
	m_Capulseinfo = std::make_shared<VPPhysics::CapsuleColliderInfo>(capsuleinfo);
}

inline void RigidBody::SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo)
{
	m_Capulseinfo.reset();
	m_Boxinfo.reset();
	m_Convexinfo.reset();
	m_Sphereinfo.reset();
	m_Boxinfo = std::make_shared<VPPhysics::BoxColliderInfo>(boxinfo);
}

inline void RigidBody::SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo)
{
	m_Capulseinfo.reset();
	m_Boxinfo.reset();
	m_Convexinfo.reset();
	m_Sphereinfo.reset();
	m_Convexinfo = std::make_shared<VPPhysics::ConvexColliderInfo>(convexinfo);
}

inline void RigidBody::SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo)
{
	m_Capulseinfo.reset();
	m_Boxinfo.reset();
	m_Convexinfo.reset();
	m_Sphereinfo.reset();
	m_Sphereinfo = std::make_shared<VPPhysics::SphereColliderInfo>(sphereinfo);
}

