#include "pch.h"
#include "ControllerManager.h"
#include "VPPhysicsStructs.h"
#include "CapsuleController.h"
#include "Controller.h"
#include "..\VPEngine\InputManager.h"
ControllerManager::ControllerManager()
{
}

ControllerManager::~ControllerManager()
{
    m_CharectorMap.clear();
    
    PX_RELEASE(m_PxControllerManager);
}

bool ControllerManager::Initialize(physx::PxScene* scene, physx::PxPhysics* physics, CollisionManager* collisionManager)
{
    m_PxPhysics = physics;
    m_Material = m_PxPhysics->createMaterial(1.f, 1.f, 0.f);
    m_Scene = scene;
    m_CollisionManager = collisionManager;
    m_PxControllerManager = PxCreateControllerManager(*scene);
    return true;
}

bool ControllerManager::CreatController()
{
    return true;
}

bool ControllerManager::CreatCapsuleController(VPPhysics::CapsuleControllerInfo capsuleinfo, VPPhysics::PhysicsInfo physicsinfo)
{
    // ���ο� CapsuleController ����
    auto capsuleController = std::make_shared<CapsuleController>();
    // CapsuleController �ʱ�ȭ
    capsuleController->Initialize(capsuleinfo, m_PxControllerManager, m_Material, physicsinfo);
    // m_CharectorMap�� CapsuleController �߰� (EntityID�� Ű�� ���)
    m_CharectorMap.insert(std::make_pair(capsuleController->GetEntityID(), capsuleController));
    // �Լ��� ���������� ����Ǿ����� ��Ÿ���� true ��ȯ
    return true;
}


bool ControllerManager::Update(float deltatime)
{
    // m_CharectorMap�� ����ִ� ��� ������Ʈ�� �������� �ʰ� true ��ȯ
    if (m_CharectorMap.empty())
        return true;
    // m_CharectorMap�� ��� ��ƼƼ ID�� ��Ʈ�ѷ��� ��ȸ
    for (auto [entityID, controller] : m_CharectorMap)
    {
        // ��Ʈ�ѷ��� CapsuleController Ÿ������ Ȯ��
        if (controller->GetTypeID() == Reflection::GetTypeID<CapsuleController>())
        {
            // CapsuleController Ÿ������ ��Ʈ�ѷ��� ĳ����
            CapsuleController* capsuleController = static_cast<CapsuleController*>(controller.get());

            // ��Ʈ�ѷ� �̵� ���� ����
            physx::PxControllerCollisionFlags collisionFlags = capsuleController->m_Controller->move(
                capsuleController->m_Velocity * deltatime, // �ӵ� * ��Ÿ Ÿ��
                0.001f,                                   // �ּ� �̵� ���ġ
                deltatime,                                // ��Ÿ Ÿ��
                *capsuleController->GetFilters()          // ���� ����
            );

            // ��Ʈ�ѷ��� �Ʒ� ���� �浹 �÷��׸� �������� Ȯ��
            if (collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN)
                capsuleController->SetIsFall(false); // �浹 �� ���� ���� ����
            else
                capsuleController->SetIsFall(true);  // �浹���� ������ ���� ���·� ����
        }
    }
    // ������Ʈ ���� �� true ��ȯ
    return true;
}

VPPhysics::RaycastData ControllerManager::RaycastActor(uint32_t entityID, VPMath::Vector3 dir, float distance)
{
    RaycastData racastresult{};

	auto temp = GetController(entityID);
	if (!temp)
		return racastresult;
	auto tempController = temp->GetPxController();
	auto tempActor = temp->GetPxController()->getActor();
	physx::PxVec3 tempDir = { dir.x,dir.y,dir.z };
	tempDir.normalize();
#pragma region Controller ver.
	{
        // Sort hits by distance


		PxF32 max = (PxF32)distance;
		const PxU32 bufferSize = 32;                 // [in] size of 'hitBuffer'
		PxRaycastHit hitBuffer[bufferSize];          // [out] User provided buffer for results
		PxRaycastBuffer buf(hitBuffer, bufferSize);  // [out] Blocking and touching hits stored here
		if (tempDir.isZero())
			return racastresult;
		auto temppose = tempController->getPosition();
		PxVec3 controllerpose = { static_cast<float>(temppose.x), static_cast<float>(temppose.y), static_cast<float>(temppose.z) };
		bool find = m_Scene->raycast(
			controllerpose,
			tempDir,
			max,
			buf);
		if (!find)
			return racastresult;
		std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
			return a.distance < b.distance;
			});
		for (PxU32 i = 0; i < buf.nbTouches; i++)
		{
            PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
            PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
            PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������

			auto tempID = GetIDFromActor(buf.touches[i].actor);	// i��° ���˵� ����(���� ��ü)�� ���õ� ��ƼƼ�� ID�� ã��
			if (tempID == entityID)
				continue;
            if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
            {

                racastresult = { tempID, {p.x,p.y,p.z},{n.x,n.y,n.z},d };
				return racastresult;
            }
		}
		return racastresult;
	}
#pragma endregion
}

VPPhysics::RaycastData ControllerManager::RaycastActor_Offset(uint32_t entityID, VPMath::Vector3 offset, VPMath::Vector3 dir, float distance)
{
    RaycastData racastresult{};

    auto temp = GetController(entityID);
    if (!temp)
        return racastresult;

    auto tempController = temp->GetPxController();
    physx::PxVec3 tempDir = { dir.x, dir.y, dir.z };
    tempDir.normalize();

    PxF32 max = (PxF32)distance;
    const PxU32 bufferSize = 32;
    PxRaycastHit hitBuffer[bufferSize];
    PxRaycastBuffer buf(hitBuffer, bufferSize);

    if (tempDir.isZero())
        return racastresult;

    // Controller ��ġ�� offset�� ����
    auto temppose = tempController->getPosition();
    PxVec3 controllerpose = { static_cast<float>(temppose.x + offset.x),
                              static_cast<float>(temppose.y + offset.y),
                              static_cast<float>(temppose.z + offset.z) };

    // Offset�� ��ġ���� ����ĳ��Ʈ ����
    bool find = m_Scene->raycast(controllerpose, tempDir, max, buf);
    if (!find)
        return racastresult;
    std::sort(buf.touches, buf.touches + buf.nbTouches, [](const PxRaycastHit& a, const PxRaycastHit& b) {
        return a.distance < b.distance;
        });
    for (PxU32 i = 0; i < buf.nbTouches; i++)
    {
        PxVec3 p = buf.getTouch(i).position;				// i��°�� ����ĳ��Ʈ�� ���� ���˵� ������ ��ġ�� ������
        PxVec3 n = buf.getTouch(i).normal;					// i��° ���˵� ������ ���� ����(ǥ���� ����)�� ������
        PxF32 d = buf.getTouch(i).distance;					// i��° ���˵� ���������� �Ÿ��� ������
        auto tempID = GetIDFromActor(buf.touches[i].actor);
        if (tempID == entityID)
            continue;

        if (!(buf.touches[i].shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE))
        {

            racastresult = { tempID, {p.x,p.y,p.z},{n.x,n.y,n.z},d };
            return racastresult;
        }
    }

    return racastresult;
}

void ControllerManager::ResizeCapsuleControllerSize(uint32_t entityID, float radius, float height)
{
    if (!HasController(entityID))
        return;
    auto controller = GetController(entityID);
    if (Reflection::IsSameType<CapsuleController>(controller->GetTypeID()))
    {
        auto Capsulecontroller = dynamic_cast<CapsuleController*>(controller);
        Capsulecontroller->ResizeCapsuleControllerSize(radius, height);
    }
}

bool ControllerManager::RemoveController(const unsigned int& id)
{
    auto controller = m_CharectorMap.find(id);
    if (controller != m_CharectorMap.end())
    {
        m_CharectorMap.erase(controller);
        return true;
    }
    return false;
}

bool ControllerManager::HasController(uint32_t entityID)
{
    return m_CharectorMap.count(entityID) > 0;
}

Controller* ControllerManager::GetController(uint32_t entityID)
{
    // ���� entityID�� m_CharectorMap�� �����ϴ��� Ȯ��
    auto it = m_CharectorMap.find(entityID);
    if (it == m_CharectorMap.end())
        return nullptr;
    auto controller = it->second;  // �˻��� ����� ���� ������
    return it->second.get();
}

physx::PxController* ControllerManager::GetPxController(uint32_t entityID)
{
    // ���� entityID�� m_CharectorMap�� �����ϴ��� Ȯ��
    auto it = m_CharectorMap.find(entityID);
    if (it == m_CharectorMap.end())
        return nullptr;
    auto controller = it->second;  // �˻��� ����� ���� ������
    return controller->GetPxController();
}

uint32_t ControllerManager::GetIDFromActor(physx::PxRigidActor* Actor)
{
    auto key = static_cast<VPPhysics::USERDATA*>(Actor->userData);
    return key->entityID;
}
