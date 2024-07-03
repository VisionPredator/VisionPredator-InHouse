#pragma once
#include "VPPhysicsStructs.h"
class RigidBody
{
public:
	RigidBody(VPPhysics::EColliderType colltype,uint32_t entityId, uint32_t layerNumber );
	~RigidBody();


	inline const uint32_t GetID() const;
	uint32_t m_EntityID{};
	VPPhysics::EColliderType m_ColliderType{};
	VPMath::Vector3 mScale{1,1,1};
	uint32_t m_LayerNum{};

	float m_Radius = 1.f;
	float m_HalfHeight = 1.f;
	physx::PxVec3 m_Extent{1,1,1};
};

const unsigned int RigidBody::GetID() const
{
	return m_EntityID;
}