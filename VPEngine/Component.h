#pragma once                                                                                                                                                                                                                            
#include "Entity.h"
//#include "SceneManager.h"
struct Component
{
    Component() = default;
    virtual ~Component()
    {
        OwnedEntity.reset(); // Reset the weak pointer for safety
    }

    virtual void SerializeComponent(nlohmann::json& json) const {}
    virtual std::shared_ptr<Component> DeserializeComponent(const nlohmann::json& json, std::shared_ptr<Entity> parentEntity) const { return nullptr; }
    virtual std::shared_ptr<Component> AddComponent(std::shared_ptr<Entity> parentEntity) { return nullptr; }

    template <typename T>
    T* GetComponent()
    {
        auto sharedEntity = OwnedEntity.lock();
        return sharedEntity ? sharedEntity->GetComponent<T>() : nullptr;
    }

    template <typename T>
    bool HasComponent()
    {
        auto sharedEntity = OwnedEntity.lock();
        return sharedEntity ? sharedEntity->HasComponent<T>() : false;
    }

    uint32_t GetEntityID()
    {
        auto sharedEntity = OwnedEntity.lock();
        return sharedEntity ? sharedEntity->GetEntityID() : 0;
    }

    std::shared_ptr<Entity> GetEntity()
    {
        return OwnedEntity.lock();
    }

    void SetEntity(std::shared_ptr<Entity> entity)
    {
        OwnedEntity = entity;
    }

    virtual entt::meta_handle GetHandle() = 0;

protected:
    std::weak_ptr<Entity> OwnedEntity;
    friend class SceneManager;
};