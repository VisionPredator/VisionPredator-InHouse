#pragma once                                                                                                                                                                                                                            
#include "Entity.h"
//#include "SceneManager.h"
struct Component
{
	Component();
	virtual ~Component()
	{
		OwnedEntity = nullptr; // Then set the pointer to nullptr (mostly for safety in this scope)
	}

	virtual void SerializeComponent(nlohmann::json& json) const {};
	virtual void* DeserializeComponent(const nlohmann::json json,Entity* parentEntity) const { return nullptr; }
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
	Entity* GetEntity() { return OwnedEntity; }
	void SetEntity(Entity* entity) { OwnedEntity = entity; }
	virtual entt::meta_handle GetHandle() abstract;


protected:
	Entity* OwnedEntity = nullptr;
	friend class SceneManager;
};

