#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void Scene::Clear()
{
    SceneName.clear();

    // EntityMap�� ��ƼƼ���� �����մϴ�.
    EntityMap.clear();  // shared_ptr�� ����ϹǷ� ������� delete�� �ʿ� �����ϴ�.

    // m_ComponentPool�� ������Ʈ���� �����մϴ�.
    for (auto& pool : m_ComponentPool) {
        for (auto* component : pool.second) {
            // ������Ʈ�� nullptr�� �ƴ� ��� �����մϴ�.
            if (component) {
                delete component;  // Component�� �����մϴ�.
                component = nullptr;  // ���� �� �����͸� nullptr�� �����մϴ�.
            }
        }
        pool.second.clear();  // ���͸� ���ϴ�.
    }
    m_ComponentPool.clear();  // ���� ���ϴ�.
}