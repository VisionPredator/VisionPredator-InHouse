#include "pch.h"
#include "Entity.h"
#include "Components.h"
Entity::Entity(uint32_t entityID)
{
	m_EntityID = entityID;
}

Entity::~Entity()
{
	for (auto& comp:m_OwnedComp)
	{
		comp.second = nullptr;
		delete comp.second;
	}
	m_OwnedComp.clear();
}
void Entity::AddComponentToMap(Component* comp)
{
	/// 메타를 사용하여 T를 타입추론해서 ID값에 해당하는 컴포넌트를 추가할수 있을듯함.
	if (HasComponent(comp->GetTypeID()))
	{
	VP_ASSERT(false, "컴포넌트가 존재합니다");
	return;
	}
	m_OwnedComp[comp->GetTypeID()] = comp;
}