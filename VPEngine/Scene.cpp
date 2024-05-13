#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
void VisPred::Scene::Clear()
{
    SceneName.clear();
	for (auto& entry : EntityMap) {
		delete entry.second;  // Delete dynamically allocated Entity objects
	}
	EntityMap.clear();  // Clear the map after deleting all entities
    // Assuming m_ComponentPool might have shared or unique components
    for (auto& pool : m_ComponentPool) {
        for (auto* component : pool.second) {
            // Since components might be shared or already deleted, check if not null
            if (component) {
                delete component;  // Delete the Component
                component = nullptr; // Set the pointer to nullptr after deletion
            }
        }
        pool.second.clear();  // Clear each vector in the pool
    }
    m_ComponentPool.clear();  // Clear the component pool map
}