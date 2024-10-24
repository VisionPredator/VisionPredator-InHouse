#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void Scene::Clear()
{
	// 씬 이름 초기화
	SceneName.clear();

	// 모든 엔티티와 그 컴포넌트를 삭제합니다.
	for (auto& entry : EntityMap)
		entry.second.reset();  // 엔티티와 그 컴포넌트가 shared_ptr로 관리되므로 reset()으로 해제합니다.
	EntityMap.clear();  // 엔티티 맵을 초기화합니다.

	// 컴포넌트 풀을 초기화합니다.
	for (auto& pool : m_ComponentPool)
	{
		for (auto& weakComp : pool.second)
		{
			if (auto sharedComp = weakComp.lock())  // weak_ptr를 shared_ptr로 변환하여 유효한지 확인합니다.
				sharedComp.reset();  // 유효하다면 reset()으로 해제합니다.
		}
		pool.second.clear();  // 각 벡터를 초기화합니다.
	}
	m_ComponentPool.clear();  // 컴포넌트 풀 맵을 초기화합니다.

	// 물리 정보 초기화가 필요한 경우 초기화합니다.
	ScenePhysicInfo = VPPhysics::PhysicsInfo{};
}