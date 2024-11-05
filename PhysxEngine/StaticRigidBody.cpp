#include "pch.h"
#include "StaticRigidBody.h"


StaticRigidBody::StaticRigidBody(VPPhysics::BoxColliderInfo info, EColliderType type, const PhysicsInfo& engininfo):RigidBody(info, type, engininfo)
{
}

StaticRigidBody::StaticRigidBody(VPPhysics::CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo)
{
}

StaticRigidBody::StaticRigidBody(VPPhysics::SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo)
{
}

StaticRigidBody::StaticRigidBody(VPPhysics::ConvexColliderInfo info, EColliderType type, const  PhysicsInfo& engininfo) :RigidBody(info, type, engininfo)
{
}

StaticRigidBody::~StaticRigidBody()
{
	
	physx::PxShape* shape;
	m_StaticRigid->getShapes(&shape, 1);
	physx::PxMaterial* material = nullptr;
	if (shape)
	{
		PxU32 materialCount = shape->getNbMaterials();
		PxMaterial** materials = new PxMaterial * [materialCount];
		shape->getMaterials(materials, materialCount);
		m_StaticRigid->detachShape(*shape);
		for (PxU32 i = 0; i < materialCount; ++i)
		{
			materials[i]->release();
		}
		delete[] materials;
	}
	m_StaticRigid->release();
}

bool StaticRigidBody::Initialize(physx::PxShape* shape, physx::PxPhysics* physics)
{
	if (m_ColliderType == EColliderType::COLLISION)
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	}
	else
	{
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}
	//data->myId = m_EntityID;
	//data->myLayerNumber = m_LayerNum;
	shape->userData = &m_UserData;
	shape->setContactOffset(0.02f);
	shape->setRestOffset(0.01f);
	physx::PxTransform transform;
	VPPhysics::ColliderInfo colliderInfo{};
	if (m_Boxinfo)
		colliderInfo = m_Boxinfo->colliderInfo;
	else if (m_Sphereinfo)
			colliderInfo= m_Sphereinfo->colliderInfo;
	else if (m_Convexinfo)
		colliderInfo = m_Convexinfo->colliderInfo;
	else if (m_Capulseinfo)
		colliderInfo = m_Capulseinfo->colliderInfo;
	else
		VP_ASSERT(false, "rigidbody 초기화 오류!");

	transform.p = {
		colliderInfo.WorldLocation.x,
		colliderInfo.WorldLocation.y,
		colliderInfo.WorldLocation.z
	};
	transform.q.x = colliderInfo.WorldQuaternion.x;
	transform.q.y = colliderInfo.WorldQuaternion.y;
	transform.q.z = colliderInfo.WorldQuaternion.z;
	transform.q.w = colliderInfo.WorldQuaternion.w;

	m_StaticRigid = physics->createRigidStatic(transform);
	m_StaticRigid->userData = &m_UserData;
	if (m_StaticRigid == nullptr)
		return false;
	if (!m_StaticRigid->attachShape(*shape))
	{
		assert(shape->getReferenceCount() == 1);
		return false;
	}
	return true;
}

