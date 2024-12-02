#include "pch.h"
#include "StaticRigidBody.h"


StaticRigidBody::StaticRigidBody(VPPhysics::BoxColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
StaticRigidBody::StaticRigidBody(VPPhysics::CapsuleColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
StaticRigidBody::StaticRigidBody(VPPhysics::SphereColliderInfo info, EColliderType type, const PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}
StaticRigidBody::StaticRigidBody(VPPhysics::ConvexColliderInfo info, EColliderType type, const  PhysicsInfo& engininfo) :RigidBody(info, type, engininfo) {}


StaticRigidBody::~StaticRigidBody()
{
	// RigidBody�� ����� shape ��������
	physx::PxShape* shape;
	m_StaticRigid->getShapes(&shape, 1);
	physx::PxMaterial* material = nullptr; // ����� Material ������ �ʱ�ȭ
	if (shape) // shape�� ��ȿ�� ���
	{
		// shape�� ����� material�� ���� ��������
		PxU32 materialCount = shape->getNbMaterials();
		// material ������ŭ �迭 �Ҵ�
		PxMaterial** materials = new PxMaterial * [materialCount];
		// shape�� ����� ��� material ��������
		shape->getMaterials(materials, materialCount);
		// shape�� RigidBody���� �и�
		m_StaticRigid->detachShape(*shape);
		// ��� material �޸� ����
		for (PxU32 i = 0; i < materialCount; ++i)
		{
			materials[i]->release(); // material ��ü ����
		}
		// material �迭 ����
		delete[] materials;
	}
	// RigidBody ��ü ����
	m_StaticRigid->release();
}

bool StaticRigidBody::Initialize(physx::PxShape* shape, physx::PxPhysics* physics)
{
	// �ݶ��̴� Ÿ���� COLLISION�� ���
	if (m_ColliderType == EColliderType::COLLISION)
		// �浹 �ùķ��̼ǿ� ����� �� �ֵ��� ����
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
	else
	{
		// �ùķ��̼� ��� ��Ȱ��ȭ
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
		// Ʈ���� ��� Ȱ��ȭ
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	// shape�� ����� �����͸� m_UserData�� ����
	shape->userData = &m_UserData;
	// ���� ������ ���� (�浹 �� �ν� �Ÿ��� ����)
	shape->setContactOffset(0.02f);
	// ���� ������ ���� (ǥ�� �� ������ ����)
	shape->setRestOffset(0.01f);
	// �ʱ�ȭ �� ����� ��ȯ ������ �浹 ���� �غ�
	physx::PxTransform transform;
	VPPhysics::ColliderInfo colliderInfo{};
	// �پ��� �ݶ��̴� Ÿ�Կ� ���� colliderInfo ����
	if (m_Boxinfo)
		colliderInfo = m_Boxinfo->colliderInfo; // Box �ݶ��̴� ����
	else if (m_Sphereinfo)
		colliderInfo = m_Sphereinfo->colliderInfo; // Sphere �ݶ��̴� ����
	else if (m_Convexinfo)
		colliderInfo = m_Convexinfo->colliderInfo; // Convex �ݶ��̴� ����
	else if (m_Capulseinfo)
		colliderInfo = m_Capulseinfo->colliderInfo; // Capsule �ݶ��̴� ����
	else
		VP_ASSERT(false, "rigidbody �ʱ�ȭ ����!"); // ���� ��Ȳ ó��

	// ��ȯ ���� ���� (��ġ �� ȸ��)
	transform.p = {
		colliderInfo.WorldLocation.x,
		colliderInfo.WorldLocation.y,
		colliderInfo.WorldLocation.z
	};
	transform.q.x = colliderInfo.WorldQuaternion.x;
	transform.q.y = colliderInfo.WorldQuaternion.y;
	transform.q.z = colliderInfo.WorldQuaternion.z;
	transform.q.w = colliderInfo.WorldQuaternion.w;

	// ���� ��ü ����
	m_StaticRigid = physics->createRigidStatic(transform);
	m_StaticRigid->userData = &m_UserData; // ����� ������ ����

	// ���� ��ü ���� ���� �� false ��ȯ
	if (m_StaticRigid == nullptr)
		return false;

	// ������ ��ü�� shape ����
	if (!m_StaticRigid->attachShape(*shape))
	{
		// ���� ���� �� ���� ī��Ʈ�� 1���� Ȯ�� �� false ��ȯ
		assert(shape->getReferenceCount() == 1);
		return false;
	}

	return true; // �ʱ�ȭ ����
}
