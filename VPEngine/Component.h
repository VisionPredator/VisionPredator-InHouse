#pragma once                                                                                                                                                                                                                            
#include "Entity.h"
#include "EntityManager.h"
struct Component
{
	Component() = default;
	virtual ~Component()
	{
		OwnedEntity = nullptr; // Then set the pointer to nullptr (mostly for safety in this scope)
	}
	Entity* OwnedEntity = nullptr;

	virtual void SerializeComponent(nlohmann::json& json) const {};
	virtual void* DeserializeComponent(const nlohmann::json json, EntityManager* entityManager, uint32_t entityID) const { return nullptr; }

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
};

template <typename T>
class IDType
{
public:
	static entt::id_type getTypeID()
	{
		static entt::id_type typeID = entt::resolve<T>().id();
		return typeID;
	}
};


