#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void Scene::Clear()
{
    SceneName.clear();

    // EntityMap의 엔티티들을 제거합니다.
    EntityMap.clear();  // shared_ptr을 사용하므로 명시적인 delete가 필요 없습니다.

    // m_ComponentPool의 컴포넌트들을 제거합니다.
    for (auto& pool : m_ComponentPool) {
        for (auto* component : pool.second) {
            // 컴포넌트가 nullptr이 아닌 경우 삭제합니다.
            if (component) {
                delete component;  // Component를 삭제합니다.
                component = nullptr;  // 삭제 후 포인터를 nullptr로 설정합니다.
            }
        }
        pool.second.clear();  // 벡터를 비웁니다.
    }
    m_ComponentPool.clear();  // 맵을 비웁니다.
}