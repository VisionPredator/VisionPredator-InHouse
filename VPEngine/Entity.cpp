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
    for (auto& comp : m_OwnedComp)
    {
        comp.second.reset(); // Reset the shared_ptr to release the resource
    }
    m_OwnedComp.clear();
}

void Entity::AddComponentToMap(std::shared_ptr<Component> comp)
{
    // ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
    if (HasComponent(comp->GetHandle()->type().id()))
    {
        VP_ASSERT(false, "������Ʈ�� �����մϴ�");
        return;
    }
    m_OwnedComp[comp->GetHandle()->type().id()] = comp;
}

std::shared_ptr<Component> Entity::AddComponent(entt::id_type compID)
{
    if (HasComponent(compID))
    {
        VP_ASSERT(false, "�̹� �ִ� Component �Դϴ�.");
        return nullptr;
    }

    auto metaType = entt::resolve(compID);

    if (metaType)
    {
        // ��Ÿ Ÿ�����κ��� �ν��Ͻ��� �����մϴ�.
        auto instance = metaType.construct();
        // Ư�� �Լ��� ã�� ȣ���մϴ�.
        auto myFunctionMeta = metaType.func("AddComponent"_hs);
        if (myFunctionMeta)
        {
            // ���� ��ƼƼ�� shared_ptr�� ��� ���� shared_from_this()�� ����մϴ�.
            std::shared_ptr<Entity> sharedThis = shared_from_this();
            entt::meta_any result = myFunctionMeta.invoke(instance, sharedThis);
            if (auto componentPtr = result.try_cast<std::shared_ptr<Component>>())
            {
                return *componentPtr;
            }
            else
            {
                VP_ASSERT(false, " �Լ� ���� ����!");
            }
        }
        else
        {
            VP_ASSERT(false, "FunctionMeta �Լ� ����!");
        }
    }
    else
    {
        VP_ASSERT(false, "resolve �Լ� ����!");
    }

    return nullptr;
}

void Entity::ReleaseComponent(std::shared_ptr<Component> comp)
{
    auto it = m_OwnedComp.find(comp->GetHandle()->type().id());
    if (it != m_OwnedComp.end())
    {
        it->second.reset(); // Reset the shared_ptr to release the resource
        m_OwnedComp.erase(it);
    }
    else
    {
        VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
    }
}
void Entity::ReleaseComponent(Component* comp)
{
    auto it = m_OwnedComp.find(comp->GetHandle()->type().id());
    if (it != m_OwnedComp.end())
    {
        it->second.reset(); // Reset the shared_ptr to release the resource
        m_OwnedComp.erase(it);
    }
    else
    {
        VP_ASSERT(false, "������Ʈ�� �������� �ʽ��ϴ�.");
    }
}