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
	/// ��Ÿ�� ����Ͽ� T�� Ÿ���߷��ؼ� ID���� �ش��ϴ� ������Ʈ�� �߰��Ҽ� ��������.
	if (HasComponent(comp->GetTypeID()))
	{
	VP_ASSERT(false, "������Ʈ�� �����մϴ�");
	return;
	}
	m_OwnedComp[comp->GetTypeID()] = comp;
}