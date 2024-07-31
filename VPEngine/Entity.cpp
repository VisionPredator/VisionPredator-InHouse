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
    // 메타를 사용하여 T를 타입추론해서 ID값에 해당하는 컴포넌트를 추가할수 있을듯함.
    if (HasComponent(comp->GetHandle()->type().id()))
    {
        VP_ASSERT(false, "컴포넌트가 존재합니다");
        return;
    }
    m_OwnedComp[comp->GetHandle()->type().id()] = comp;
}

std::shared_ptr<Component> Entity::AddComponent(entt::id_type compID)
{
    if (HasComponent(compID))
    {
        VP_ASSERT(false, "이미 있는 Component 입니다.");
        return nullptr;
    }

    auto metaType = entt::resolve(compID);

    if (metaType)
    {
        // 메타 타입으로부터 인스턴스를 생성합니다.
        auto instance = metaType.construct();
        // 특정 함수를 찾고 호출합니다.
        auto myFunctionMeta = metaType.func("AddComponent"_hs);
        if (myFunctionMeta)
        {
            // 현재 엔티티의 shared_ptr을 얻기 위해 shared_from_this()를 사용합니다.
            std::shared_ptr<Entity> sharedThis = shared_from_this();
            entt::meta_any result = myFunctionMeta.invoke(instance, sharedThis);
            if (auto componentPtr = result.try_cast<std::shared_ptr<Component>>())
            {
                return *componentPtr;
            }
            else
            {
                VP_ASSERT(false, " 함수 리턴 실패!");
            }
        }
        else
        {
            VP_ASSERT(false, "FunctionMeta 함수 실패!");
        }
    }
    else
    {
        VP_ASSERT(false, "resolve 함수 실패!");
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
        VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
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
        VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
    }
}