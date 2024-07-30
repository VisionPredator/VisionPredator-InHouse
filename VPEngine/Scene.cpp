#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void Scene::Clear()
{
	// �� �̸� �ʱ�ȭ
	SceneName.clear();

	// ��� ��ƼƼ�� �� ������Ʈ�� �����մϴ�.
	for (auto& entry : EntityMap)
		entry.second.reset();  // ��ƼƼ�� �� ������Ʈ�� shared_ptr�� �����ǹǷ� reset()���� �����մϴ�.
	EntityMap.clear();  // ��ƼƼ ���� �ʱ�ȭ�մϴ�.

	// ������Ʈ Ǯ�� �ʱ�ȭ�մϴ�.
	for (auto& pool : m_ComponentPool)
	{
		for (auto& weakComp : pool.second)
		{
			if (auto sharedComp = weakComp.lock())  // weak_ptr�� shared_ptr�� ��ȯ�Ͽ� ��ȿ���� Ȯ���մϴ�.
				sharedComp.reset();  // ��ȿ�ϴٸ� reset()���� �����մϴ�.
		}
		pool.second.clear();  // �� ���͸� �ʱ�ȭ�մϴ�.
	}
	m_ComponentPool.clear();  // ������Ʈ Ǯ ���� �ʱ�ȭ�մϴ�.

	// ���� ���� �ʱ�ȭ�� �ʿ��� ��� �ʱ�ȭ�մϴ�.
	ScenePhysicInfo = VPPhysics::PhysicsInfo{};
}