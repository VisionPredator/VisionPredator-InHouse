#pragma once
#include "pch.h"
#include "EventManager.h"
struct Component;

class Entity : public std::enable_shared_from_this<Entity>
{
public:
    Entity();
    Entity(uint32_t entityID);
    Entity(const Entity& other) = default;
    ~Entity();

    template<typename T>
    T* GetComponent() requires std::derived_from<T, Component>
    {
        return std::static_pointer_cast<T>(m_OwnedComp[Reflection::GetTypeID<T>()]).get();

    }

    Component* GetComponent(entt::id_type compID)
    {
        return m_OwnedComp[compID].get();
    }

    template<typename T>
    bool HasComponent() const
    {
        return m_OwnedComp.find(Reflection::GetTypeID<T>()) != m_OwnedComp.end();
    }

    bool HasComponent(entt::id_type compID) const
    {
        return m_OwnedComp.find(compID) != m_OwnedComp.end();
    }

    const uint32_t GetEntityID() { return m_EntityID; }

    void AddComponentToMap(std::shared_ptr<Component> comp);

    template<typename T>
    std::shared_ptr<T> AddComponent(bool Immediately = false)
    {
        if (HasComponent<T>())
        {
            VP_ASSERT(false, "�̹� �ִ� Component �Դϴ�.");
            return nullptr;
        }
        std::shared_ptr<T> newcomp = std::make_shared<T>();
        AddComponentToMap(newcomp);
        newcomp->SetEntity(this); // Use shared_from_this to set the entity
        if (!Immediately)
            EventManager::GetInstance().ScheduleEvent("OnAddCompToScene", std::static_pointer_cast<Component>(newcomp));
        else
            EventManager::GetInstance().ImmediateEvent("OnAddCompToScene", std::static_pointer_cast<Component>(newcomp));
        return newcomp;
    }

    std::shared_ptr<Component> AddComponent(entt::id_type compID);

    template<typename T>
    void RemoveComponent()
    {
        EventManager::GetInstance().ScheduleEvent("OnRemoveComp_Scene", GetComponent(Reflection::GetTypeID<T>()));
    }

    void RemoveComponent(entt::id_type compID)
    {
        EventManager::GetInstance().ScheduleEvent("OnRemoveComp_Scene", GetComponent(compID));
    }

private:

    std::vector<std::shared_ptr<Component>> GetOwnedComponents()
    {
        std::vector<std::shared_ptr<Component>> componentPool;
        for (auto& [id,component] : m_OwnedComp)
        {
            componentPool.push_back(component);
        }
        return componentPool;
    }

    void SetEntityID(uint32_t entityid) { m_EntityID = entityid; }



    void ReleaseComponent(std::shared_ptr<Component> comp);
    void ReleaseComponent(Component* comp);

    operator bool() const { return m_EntityID; }
    operator uint32_t() const { return m_EntityID; }
    bool operator==(const Entity& other) const
    {
        return m_EntityID == other.m_EntityID;/* && m_Scene == other.m_Scene;*/
    }
    bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

    uint32_t m_EntityID{};
    std::unordered_map<entt::id_type, std::shared_ptr<Component>> m_OwnedComp;
    friend class SceneSerializer;
    friend class SceneManager;
};
