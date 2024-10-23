#include "pch.h"
#include "Entity.h"
#include "Components.h"

Entity::Entity()
{
}

Entity::Entity(uint32_t entityID)
{
    m_EntityID = entityID;
}

Entity::~Entity()
{
    for (auto comp : m_OwnedComp)
    {
        comp.second.reset();
    }
    m_OwnedComp.clear(); // shared_ptr�� �Ҹ��ڰ� �ڵ����� �ڿ��� �����մϴ�.
}


void Entity::DestorySelf(bool Immidiate)
{
    if (!Immidiate)
        EventManager::GetInstance().ScheduleEvent("OnDestroyEntity",m_EntityID);
    else
        EventManager::GetInstance().ImmediateEvent("OnDestroyEntity", m_EntityID);
}

void Entity::AddComponentToMap(std::shared_ptr<Component> comp)
{
    auto compID = comp->GetHandle()->type().id();
    if (HasComponent(compID))
    {
        VP_ASSERT(false, "������Ʈ�� �����մϴ�");
        return;
    }
    m_OwnedComp[compID] = std::move(comp);
}
std::shared_ptr<Component> Entity::AddComponent(entt::id_type compID)
{
    if (HasComponent(compID))
    {
        VP_ASSERT(false, "�̹� �ִ� Component �Դϴ�.");
        return nullptr;
    }

    auto metaType = entt::resolve(compID);
    if (!metaType)
    {
        VP_ASSERT(false, "resolve �Լ� ����!");
        return nullptr;
    }

    auto instance = metaType.construct();
    auto myFunctionMeta = metaType.func("AddComponent"_hs);
    if (!myFunctionMeta)
    {
        VP_ASSERT(false, "FunctionMeta �Լ� ����!");
        return nullptr;
    }

    //auto sharedThis = shared_from_this();
    auto result = myFunctionMeta.invoke(instance, this);
    if (auto componentPtr = result.try_cast<std::shared_ptr<Component>>())
    {
        return *componentPtr;
    }

    VP_ASSERT(false, " �Լ� ���� ����!");
    return nullptr;
}


void Entity::removeComponent(std::shared_ptr<Component> comp)
{
    auto compID = comp->GetHandle()->type().id();
    auto it = m_OwnedComp.find(compID);
    if (it != m_OwnedComp.end())
    {
        m_OwnedComp.erase(it); // shared_ptr�� �Ҹ��ڰ� �ڵ����� �ڿ��� �����մϴ�.
    }
    else
    {
        VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
    }
}
void Entity::removeComponent(Component* comp)
{
    auto compID = comp->GetHandle()->type().id();
    auto it = m_OwnedComp.find(compID);
    if (it != m_OwnedComp.end())
    {
        m_OwnedComp.erase(it); // shared_ptr�� �Ҹ��ڰ� �ڵ����� �ڿ��� �����մϴ�.
    }
    else
    {
        VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
    }
}