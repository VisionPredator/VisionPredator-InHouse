#include "pch.h"
#include "PhysichResourceManager.h"
#include "ConvexMeshResource.h"
#include "VPPhysicsStructs.h"

// PhysichResourceManager Ŭ������ ������
PhysichResourceManager::PhysichResourceManager()
{
}

// PhysichResourceManager Ŭ���� �ʱ�ȭ �Լ�
// PxPhysics ��ü�� �޾� �ʱ�ȭ�ϸ�, nullptr�� ���޵Ǹ� false ��ȯ
bool PhysichResourceManager::Initialize(physx::PxPhysics* physics)
{
	if (!physics)
		return false; // ���� ���� ��ü�� ��ȿ���� ������ �ʱ�ȭ ����
	m_PxPhysics = physics; // ��ȿ�� PxPhysics ��ü�� ��� ������ ����
	return true; // �ʱ�ȭ ����
}

// PhysichResourceManager Ŭ������ �Ҹ���
PhysichResourceManager::~PhysichResourceManager()
{
	m_PxPhysics = nullptr;
}

// ConvexMeshResource�� Ű ������ �������� �Լ�
std::shared_ptr<ConvexMeshResource> PhysichResourceManager::GetConvexMeshResource(const std::wstring& key)
{
	auto it = m_ConvexMeshMap.find(key); // Ű�� �˻�
	if (it == m_ConvexMeshMap.end())
		return nullptr; // Ű�� �������� ������ nullptr ��ȯ
    return it->second; // Ű�� �����ϸ� �ش� ���ҽ��� ��ȯ
}

// ConvexMeshResource�� �ε��ϰ� �����ϴ� �Լ�
void PhysichResourceManager::LoadConvexMeshResource(const VPPhysics::ConvexMeshResourceInfo& info)
{
    // ConvexMeshResource ��ü ����
    std::shared_ptr<ConvexMeshResource> convexmesh = std::make_shared<ConvexMeshResource>(m_PxPhysics, info);
    // ������ ���ҽ��� �ʿ� ���� (FBXName�� Ű�� ���)
    m_ConvexMeshMap[info.FBXName] = convexmesh;
}

// Ư�� ConvexMeshResource�� �����ϴ��� Ȯ���ϴ� �Լ�
bool PhysichResourceManager::HasConvexMeshResource(const std::wstring& key)
{
    // �ʿ� Ű�� �����ϸ� true, �ƴϸ� false ��ȯ
    return m_ConvexMeshMap.find(key) != m_ConvexMeshMap.end();
}
