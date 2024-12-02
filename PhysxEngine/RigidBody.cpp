#include "pch.h"
#include "RigidBody.h"


#include "pch.h"
#include "RigidBody.h"

// CapsuleColliderInfo�� ������� RigidBody ������ ����
RigidBody::RigidBody(VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // �ݶ��̴� Ÿ�� ����
	, m_UserData{ info.colliderInfo.EntityID } // ����� ������ �ʱ�ȭ 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // ���� ���̾� ��ȣ ����
{
	// ���� ���� ������ ����
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// ĸ�� �ݶ��̴� ���� ����
	SetColliderinfo(info);
}

// BoxColliderInfo�� ������� RigidBody ������ ����
RigidBody::RigidBody(VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // �ݶ��̴� Ÿ�� ����
	, m_UserData{ info.colliderInfo.EntityID } // ����� ������ �ʱ�ȭ
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // ���� ���̾� ��ȣ ����
{
	// ���� ���� ������ ����
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// �ڽ� �ݶ��̴� ���� ����
	SetColliderinfo(info);
}

// ConvexColliderInfo�� ������� RigidBody ������ ����
RigidBody::RigidBody(VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // �ݶ��̴� Ÿ�� ����
	, m_UserData{ info.colliderInfo.EntityID } // ����� ������ �ʱ�ȭ 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // ���� ���̾� ��ȣ ����
{
	// ���� ���� ������ ����
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// Convex �ݶ��̴� ���� ����
	SetColliderinfo(info);
}

// SphereColliderInfo�� ������� RigidBody ������ ����
RigidBody::RigidBody(VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo)
	: m_ColliderType{ type } // �ݶ��̴� Ÿ�� ����
	, m_UserData{ info.colliderInfo.EntityID } // ����� ������ �ʱ�ȭ 
	, m_LayerNum{ info.colliderInfo.PhysicsLayer } // ���� ���̾� ��ȣ ����
{
	// ���� ���� ������ ����
	m_PhysicInfo = std::make_shared<VPPhysics::PhysicsInfo>(engininfo);
	// Sphere �ݶ��̴� ���� ����
	SetColliderinfo(info);
}

// RigidBody �Ҹ���
RigidBody::~RigidBody() {}
