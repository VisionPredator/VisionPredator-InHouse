#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void Scene::Clear()
{
    
    SceneName.clear();          // 씬 이름 초기화

   
    for (auto& entry : EntityMap)    // Clear all entities and their components
        entry.second.reset();        // Since entities and their components are managed by shared_ptr, just reset them
    EntityMap.clear();

	// Clear the component pool
	for (auto& pool : m_ComponentPool)
		if (auto sharedComp = pool.second.lock())       // Convert weak_ptr to shared_ptr and reset them if they are still valid
			sharedComp.reset();

	m_ComponentPool.clear();

	// Clear physics info if needed
	ScenePhysicInfo = VPPhysics::PhysicsInfo{};
}