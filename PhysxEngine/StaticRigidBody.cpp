#include "pch.h"
#include "StaticRigidBody.h"


StaticRigidBody::StaticRigidBody(VPPhysics::EColliderType colltype, uint32_t entityId, VPPhysics::EPhysicsLayer layerNumber)
	:RigidBody(colltype, entityId, layerNumber)
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


bool StaticRigidBody::Initialize(ColliderInfo colliderInfo, physx::PxShape* shape, physx::PxPhysics* physics)
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
	//shape->userData = data;
	shape->setContactOffset(0.02f);
	shape->setRestOffset(0.01f);
	physx::PxTransform transform;
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
	//m_StaticRigid->userData = data;
	if (m_StaticRigid == nullptr)
		return false;
	if (!m_StaticRigid->attachShape(*shape))
	{
		assert(shape->getReferenceCount() == 1);
		return false;
	}
	return true;

}

