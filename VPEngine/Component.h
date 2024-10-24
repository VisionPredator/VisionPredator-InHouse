#pragma once                                                                                                                                                                                                                            
#include "Entity.h"
//#include "SceneManager.h"
struct Component
{
    Component() = default;
    virtual ~Component()
    {
        OwnedEntity=nullptr; // Reset the weak pointer for safety
    }

    virtual void SerializeComponent(nlohmann::json& json) const {}


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

	const uint32_t GetEntityID()
	{
		return OwnedEntity->GetEntityID();
	}

    Entity* GetEntity()
    {
        return OwnedEntity;
    }

    void SetEntity(Entity* entity)
    {
        OwnedEntity = entity;
    }




    virtual entt::meta_handle GetHandle() = 0;

protected:

    template <typename T>
    std::shared_ptr<T> AddComponentToEntity(Entity* parentEntity, bool Immediately = false, bool UseAddCompToScene = true) const{
        // Call Entity's private AddComponent method
        return parentEntity->AddComponent<T>(Immediately, UseAddCompToScene);
    }


    virtual std::shared_ptr<Component> AddComponent(Entity* parentEntity) { return nullptr; }
    virtual std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, Entity* parentEntity, bool Immidiate = false,bool UseAddCompToScene=true) const { return nullptr; }
    Entity* OwnedEntity;
    friend class SceneManager;
};