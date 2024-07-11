#pragma once
#include "VPPhysicsStructs.h"
class RigidBody
{
public:
	RigidBody(VPPhysics::EColliderType colltype,uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber );
	virtual ~RigidBody(); // Make the destructor virtual

	inline const uint32_t GetID() const;
	uint32_t m_EntityID{};
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


	float GetRadius() { return m_Radius; }
	float GetHalfHeight() { return m_HalfHeight; }
	physx::PxVec3 GetExtent() { return m_Extent; }
};



const unsigned int RigidBody::GetID() const
{
	return m_EntityID;
}