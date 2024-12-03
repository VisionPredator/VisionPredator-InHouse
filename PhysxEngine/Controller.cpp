#include "pch.h"
#include "Controller.h"
#include "ControllerQueryFilterCallback.h"

// Controller Ŭ������ ������
Controller::Controller()
{
}

// Controller Ŭ������ �Ҹ���
Controller::~Controller()
{
    // m_Controller�� �����ϸ� PhysX ��ü�� ����
    PX_RELEASE(m_Controller);
    m_Material = nullptr;
}

// ��Ʈ�ѷ� �ʱ�ȭ �Լ�
// ��Ʈ�ѷ� ����(info), ���� ����(material), ���� ���� ����(physicsinfo)�� ������� �ʱ�ȭ
bool Controller::ControllerInit(VPPhysics::ControllerInfo info, physx::PxMaterial* material, VPPhysics::PhysicsInfo physicsinfo)
{
    // ��Ʈ�ѷ��� ���� ���̾ ����
    m_LayerNum = info.LayerNumber;

    // TODO: ������ ã�ƺ��� (���� ������ �ʱ�ȭ ����)
    m_FilterData = std::make_shared<PxFilterData>();
    m_FilterData->word0 = 0; // �⺻������ word0�� 0���� ����

    // �� PxFilterData ���� �� �ʱ�ȭ
    std::shared_ptr<physx::PxFilterData> data = std::make_shared<physx::PxFilterData>();
    m_Material = material;
    data->word0 = (int)m_LayerNum; // ���̾� ��ȣ�� word0�� ����
    data->word1 = physicsinfo.CollisionMatrix[(int)m_LayerNum]; // �浹 ��Ʈ������ word1�� ����

    // PxQueryFilterCallback �� PxControllerFilterCallback ����
    m_PxQueryFilterCallback = std::make_shared<ControllerQueryFilterCallback>(data);
    m_PxControllerFilterCallback = std::make_shared<MyControllerFilterCallback>();

    // PxControllerFilters�� �����Ͽ� ���� �ݹ� �� ������ ����
    m_Filters = std::make_shared<PxControllerFilters>(m_FilterData.get(), m_PxQueryFilterCallback.get(), m_PxControllerFilterCallback.get());

    // ����� �����Ϳ� ��ƼƼ ID�� ����
    m_UserData.entityID = info.EntityId;

    return true; // �ʱ�ȭ ����
}

// ��Ʈ�ѷ��� ��ġ�� �����ϴ� �Լ�
void Controller::SetPosition(const VPMath::Vector3& position)
{
    // VPMath::Vector3�� PxExtendedVec3�� ��ȯ
	physx::PxExtendedVec3 vector{ position.x,position.y,position.z };
    // ��Ʈ�ѷ��� ��ġ ����
    m_Controller->setPosition(vector);
}

// ��Ʈ�ѷ��� ���� ��ġ�� �������� �Լ�
const VPMath::Vector3 Controller::GetPosition()
{
	VPMath::Vector3 Pos = {
		static_cast<float>(m_Controller->getPosition().x)
		,static_cast<float>(m_Controller->getPosition().y)
		,static_cast<float>(m_Controller->getPosition().z)
	};
	return Pos; // ��ȯ�� ��ġ ��ȯ
}
