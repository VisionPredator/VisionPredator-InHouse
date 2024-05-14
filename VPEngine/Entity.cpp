#include "pch.h"
#include "Entity.h"
#include "Components.h"
#include "EntityManager.h"
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
