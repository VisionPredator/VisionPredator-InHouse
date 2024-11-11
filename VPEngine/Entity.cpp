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
    m_OwnedComp.clear(); // shared_ptr의 소멸자가 자동으로 자원을 해제합니다.
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
        VP_ASSERT(false, "컴포넌트가 존재합니다");
        return;
    }
    m_OwnedComp[compID] = std::move(comp);
}
std::shared_ptr<Component> Entity::AddComponent(entt::id_type compID)
{
    if (HasComponent(compID))
    {
        VP_ASSERT(false, "이미 있는 Component 입니다.");
        return nullptr;
    }

    auto metaType = entt::resolve(compID);
    if (!metaType)
    {
        VP_ASSERT(false, "resolve 함수 실패!");
        return nullptr;
    }

    auto instance = metaType.construct();
    auto myFunctionMeta = metaType.func("AddComponent"_hs);
    if (!myFunctionMeta)
    {
        VP_ASSERT(false, "FunctionMeta 함수 실패!");
        return nullptr;
    }

    //auto sharedThis = shared_from_this();
    auto result = myFunctionMeta.invoke(instance, this);
    if (auto componentPtr = result.try_cast<std::shared_ptr<Component>>())
    {
        return *componentPtr;
    }

    VP_ASSERT(false, " 함수 리턴 실패!");
    return nullptr;
}


void Entity::removeComponent(std::shared_ptr<Component> comp)
{
    auto compID = comp->GetHandle()->type().id();
    auto it = m_OwnedComp.find(compID);
    if (it != m_OwnedComp.end())
    {
        m_OwnedComp.erase(it); // shared_ptr의 소멸자가 자동으로 자원을 해제합니다.
    }
    else
    {
        VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
    }
}
void Entity::removeComponent(Component* comp)
{
    auto compID = comp->GetHandle()->type().id();
    auto it = m_OwnedComp.find(compID);
    if (it != m_OwnedComp.end())
    {
        m_OwnedComp.erase(it); // shared_ptr의 소멸자가 자동으로 자원을 해제합니다.
    }
    else
    {
        VP_ASSERT(false, "컴포넌트가 존재하지 않습니다.");
    }
}
std::shared_ptr<Entity> Entity::Clone(uint32_t entityID)
{
    std::shared_ptr<Entity> cloneEntity = std::make_shared<Entity>(entityID);

    for (const auto& [compID, compPtr] : m_OwnedComp)
    {
        std::shared_ptr<Component> newComp;

        // Children 컴포넌트인 경우, 자식 ID를 개별적으로 복사
        if (auto childrenComp = std::dynamic_pointer_cast<Children>(compPtr))
        {
            auto clonedChildrenComp = std::make_shared<Children>(*childrenComp);
            clonedChildrenComp->ChildrenID.clear(); // 자식 ID 목록 초기화

            for (const auto childID : childrenComp->ChildrenID)
            {
                // 각 자식 ID를 새로운 엔티티로 복사하여 추가
                clonedChildrenComp->ChildrenID.push_back(childID);
            }

            newComp = clonedChildrenComp;
        }
        else
        {
            newComp = compPtr->Clone();
        }

        cloneEntity->AddComponentToMap(newComp);
        newComp->SetEntity(cloneEntity.get());
    }

    return cloneEntity;
}
