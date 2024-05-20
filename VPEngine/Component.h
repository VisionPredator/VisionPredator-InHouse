#pragma once                                                                                                                                                                                                                            
#include "Entity.h"
#include "SceneManager.h"
struct Component
{
	Component() = default;
	virtual ~Component()
	{
		OwnedEntity = nullptr; // Then set the pointer to nullptr (mostly for safety in this scope)
	}

	virtual void SerializeComponent(nlohmann::json& json) const {};
	virtual void* DeserializeComponent(const nlohmann::json json, SceneManager* sceneManager, uint32_t entityID) const { return nullptr; }
	virtual Component* AddComponent(Entity* parentEntity) { return nullptr; }
	template <typename T>
	T* GetComponent()
	{
		return OwnedEntity->GetComponent<T>();
	}
	template <typename T>
	bool HasComponent()
	{
		return OwnedEntity->HasComponent<T>();
	}
	uint32_t GetEntityID()
	{
		return OwnedEntity->GetEntityID();
	}
	Entity* OwnedEntity = nullptr;
	virtual entt::id_type GetTypeID() const = 0;
protected:
	friend class SceneManager;
};

