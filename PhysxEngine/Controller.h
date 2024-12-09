#pragma once
#include "VPPhysicsStructs.h"
using namespace VPPhysics;

// ��Ʈ�ѷ� �� ���͸��� ���� ����� ���� ���� �ݹ� Ŭ����
class ControllerQueryFilterCallback;

// PxControllerFilterCallback�� ��ӹ޾� ��Ʈ�ѷ� �� ��ȣ�ۿ��� ���͸��ϴ� Ŭ����
class MyControllerFilterCallback : public physx::PxControllerFilterCallback
{
public:
    // �� ��Ʈ�ѷ� a�� b�� ��ȣ�ۿ��� �� �ִ��� ���θ� �����ϴ� �Լ�
    virtual bool filter(const physx::PxController& a, const physx::PxController& b) override {
        // �� ��Ʈ�ѷ��� Shape�� ������ (�� ��Ʈ�ѷ��� �ϳ��� Shape�� �����ٰ� ����)
        physx::PxShape* shapeA = nullptr;
        physx::PxShape* shapeB = nullptr;
        a.getActor()->getShapes(&shapeA, 1);
        b.getActor()->getShapes(&shapeB, 1);

        // Shape�� �ϳ��� nullptr�̸� ��ȣ�ۿ��� ����
        if (!shapeA || !shapeB) {
            return false;
        }

        // Shape�� FilterData�� ������
        physx::PxFilterData filterDataA = shapeA->getSimulationFilterData();
        physx::PxFilterData filterDataB = shapeB->getSimulationFilterData();

        // ���͸� �� (CustomSimulationFilterShader�� ����)
        bool shouldInteract = (((1 << filterDataA.word0) & filterDataB.word1) > 0) &&
            (((1 << filterDataB.word0) & filterDataA.word1) > 0);

        // �� ��Ʈ�ѷ��� ��ȣ�ۿ� �����ϸ� true ��ȯ
        return shouldInteract;
    }
};

// ��Ʈ�ѷ� Ŭ���� ����
class Controller
{
public:
    // �⺻ ������ �� �Ҹ���
    Controller();
    ~Controller();

    // ��Ʈ�ѷ� �ʱ�ȭ �Լ�
    // ����(info), ����(material), ���� ����(physicsinfo)�� �Է¹޾� �ʱ�ȭ
    bool ControllerInit(VPPhysics::ControllerInfo info, physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo);

    // ��Ʈ�ѷ��� ��ġ�� �����ϴ� �Լ�
    void SetPosition(const VPMath::Vector3& position);

    // ���� ���¸� �����ϴ� �Լ� (inline)
    inline void SetIsFall(bool isfall);

    // �ӵ��� �����ϴ� �Լ� (inline)
    inline void SetVelocity(VPMath::Vector3 velocity);

    // ��Ʈ�ѷ��� ���� ��ġ�� �������� �Լ�
    const VPMath::Vector3 GetPosition();

    // �ӵ��� �������� �Լ� (inline)
    inline VPMath::Vector3 GetVelocity();

    // ���� ���¸� �������� �Լ� (inline)
    inline bool GetIsFall();

    // PxController �����͸� ��ȯ�ϴ� �Լ�
    physx::PxController* GetPxController() { return m_Controller; }

    // ��Ʈ�ѷ� Ÿ�� ID�� ��ȯ�ϴ� �Լ�
    virtual entt::id_type GetTypeID() const { return Reflection::GetTypeID<Controller>(); }

    // ��ƼƼ ID�� �������� �Լ� (inline)
    inline uint32_t GetEntityID();

    // ���͸� �������� �Լ� (inline)
    inline physx::PxControllerFilters* GetFilters();

    // ��� ����
    USERDATA m_UserData{}; // ����� ������
    physx::PxController* m_Controller{}; // ��Ʈ�ѷ� ��ü
    physx::PxControllerCollisionFlags m_Collisionflag; // �浹 �÷��� ���� ���� m_Collisionflag; // �ݸ��� üũ
    VPPhysics::EPhysicsLayer m_LayerNum{}; // ���� ���̾� ����
    physx::PxMaterial* m_Material{}; // ���� ����
    std::shared_ptr<PxFilterData> m_FilterData{}; // ���� ������
    std::shared_ptr<ControllerQueryFilterCallback> m_PxQueryFilterCallback{}; // ���� ���� �ݹ�
    std::shared_ptr<MyControllerFilterCallback> m_PxControllerFilterCallback{}; // ��Ʈ�ѷ� ���� �ݹ�
    std::shared_ptr<PxControllerFilters> m_Filters{}; // ����

    physx::PxVec3 m_Velocity{}; // �ӵ�
    bool m_IsFall{}; // ���� ����
};

// ��ƼƼ ID�� ��ȯ�ϴ� �Լ� (inline)
inline uint32_t Controller::GetEntityID()
{
    return m_UserData.entityID;
}

// ���͸� ��ȯ�ϴ� �Լ� (inline)
inline physx::PxControllerFilters* Controller::GetFilters()
{
    return m_Filters.get();
}

// ���� ���¸� �����ϴ� �Լ� (inline)
inline void Controller::SetIsFall(bool isfall)
{
    m_IsFall = isfall;
}

// ���� ���¸� �������� �Լ� (inline)
inline bool Controller::GetIsFall()
{
    return m_IsFall;
}

// �ӵ��� �����ϴ� �Լ� (inline)
inline void Controller::SetVelocity(VPMath::Vector3 velocity)
{
    m_Velocity = { velocity.x, velocity.y, velocity.z };
}

// �ӵ��� �������� �Լ� (inline)
inline VPMath::Vector3 Controller::GetVelocity()
{
    return { m_Velocity.x, m_Velocity.y, m_Velocity.z };
}
