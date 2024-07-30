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
        return FindComponent<T>();
    }

    Component* GetComponent(entt::id_type compID)
    {
        return FindComponent(compID);
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
            VP_ASSERT(false, "이미 있는 Component 입니다.");
            return nullptr;
        }
        std::shared_ptr<T> newcomp = std::make_shared<T>();
        AddComponentToMap(newcomp);
        newcomp->SetEntity(shared_from_this()); // Use shared_from_this to set the entity
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
    std::vector<std::shared_ptr<Component>> GetOwnedComponents() const
    {
        std::vector<std::shared_ptr<Component>> components;
        for (const auto& [id, comp] : m_OwnedComp)
        {
            components.push_back(comp);
        }
        return components;
    }


    std::vector<std::shared_ptr<Component>> GetOwnedComponent()
    {
        std::vector<std::shared_ptr<Component>> tempComponentPool;
        for (auto& component : m_OwnedComp)
        {
            tempComponentPool.push_back(component.second);
        }
        return tempComponentPool;
    }

    void SetEntityID(uint32_t entityid) { m_EntityID = entityid; }

    template<typename T>
    T* FindComponent()
    {
        auto it = m_OwnedComp.find(Reflection::GetTypeID<T>());
        return (it != m_OwnedComp.end()) ? std::static_pointer_cast<T>(it->second).get() : nullptr;
    }

    Component* FindComponent(entt::id_type compID)
    {
        auto it = m_OwnedComp.find(compID);
        if (it == m_OwnedComp.end())
        {
            VP_ASSERT(false, "Component not found in m_OwnedComp");
            return nullptr;
        }
        return it->second.get();
    }

    void ReleaseComponent(std::shared_ptr<Component> comp);

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