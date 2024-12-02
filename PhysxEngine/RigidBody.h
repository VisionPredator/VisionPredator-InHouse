#pragma once
#include "VPPhysicsStructs.h"

class RigidBody
{
public:
    // �پ��� ColliderInfo�� ������� RigidBody ������ ����
    RigidBody(VPPhysics::CapsuleColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::BoxColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::ConvexColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);
    RigidBody(VPPhysics::SphereColliderInfo info, VPPhysics::EColliderType type, const VPPhysics::PhysicsInfo& engininfo);

    // �Ҹ��ڸ� ���� �Լ��� �����Ͽ� ��� �� �����ϰ� ���
    virtual ~RigidBody();

    // ����Ƽ ID�� ��ȯ�ϴ� �Լ�
    inline const uint32_t GetID() const;

    // ���� �������� ����� ����� ������ (����Ƽ ID, Ȱ��ȭ ����)
    VPPhysics::USERDATA m_UserData{};
    // �ݶ��̴� Ÿ�� (COLLISION �Ǵ� TRIGGER)
    VPPhysics::EColliderType m_ColliderType{};

    // ���� ���̾� ��ȣ
    VPPhysics::EPhysicsLayer m_LayerNum{};

    // �ݶ��̴� ���� ���� �Լ� (Ÿ�Ժ��� ���� ����)
    void SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo);
    void SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo);
    void SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo);
    void SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo);


    // RigidBody Ÿ�� ID�� ��ȯ�ϴ� ���� �Լ� (Reflection ���̺귯���� ���)
	virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<RigidBody>(); }

    // �ݶ��̴� ������ �����ϴ� ����Ʈ ������
    std::shared_ptr<VPPhysics::CapsuleColliderInfo> m_Capulseinfo; // ĸ�� �ݶ��̴� ����
    std::shared_ptr<VPPhysics::BoxColliderInfo> m_Boxinfo;         // �ڽ� �ݶ��̴� ����
    std::shared_ptr<VPPhysics::ConvexColliderInfo> m_Convexinfo;   // Convex �ݶ��̴� ����
    std::shared_ptr<VPPhysics::SphereColliderInfo> m_Sphereinfo;  // Sphere �ݶ��̴� ����
    std::shared_ptr<VPPhysics::PhysicsInfo> m_PhysicInfo;         // ���� ���� ����
};

// GetID: ����Ƽ ID�� ��ȯ
const unsigned int RigidBody::GetID() const
{
    return m_UserData.entityID;
}

// SetColliderinfo: ĸ�� �ݶ��̴� ������ ����
inline void RigidBody::SetColliderinfo(const VPPhysics::CapsuleColliderInfo& capsuleinfo)
{
    // �ٸ� �ݶ��̴� ������ �ʱ�ȭ
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // ĸ�� �ݶ��̴� ���� ����
    m_Capulseinfo = std::make_shared<VPPhysics::CapsuleColliderInfo>(capsuleinfo);
}

// SetColliderinfo: �ڽ� �ݶ��̴� ������ ����
inline void RigidBody::SetColliderinfo(const VPPhysics::BoxColliderInfo& boxinfo)
{
    // �ٸ� �ݶ��̴� ������ �ʱ�ȭ
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // �ڽ� �ݶ��̴� ���� ����
    m_Boxinfo = std::make_shared<VPPhysics::BoxColliderInfo>(boxinfo);
}

// SetColliderinfo: Convex �ݶ��̴� ������ ����
inline void RigidBody::SetColliderinfo(const VPPhysics::ConvexColliderInfo& convexinfo)
{
    // �ٸ� �ݶ��̴� ������ �ʱ�ȭ
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // Convex �ݶ��̴� ���� ����
    m_Convexinfo = std::make_shared<VPPhysics::ConvexColliderInfo>(convexinfo);
}

// SetColliderinfo: Sphere �ݶ��̴� ������ ����
inline void RigidBody::SetColliderinfo(const VPPhysics::SphereColliderInfo& sphereinfo)
{
    // �ٸ� �ݶ��̴� ������ �ʱ�ȭ
    m_Capulseinfo.reset();
    m_Boxinfo.reset();
    m_Convexinfo.reset();
    m_Sphereinfo.reset();
    // Sphere �ݶ��̴� ���� ����
    m_Sphereinfo = std::make_shared<VPPhysics::SphereColliderInfo>(sphereinfo);
}
