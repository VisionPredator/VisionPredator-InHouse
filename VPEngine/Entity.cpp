#include "pch.h"
#include "Entity.h"
#include "Components.h"
#include "EntityManager.h"
VisPred::Entity::Entity(uint32_t entityID)
{
	m_EntityID = entityID;
}

VisPred::Entity::~Entity()
{
	for (auto& comp:m_OwnedComp)
	{
		comp.second = nullptr;
		delete comp.second;
	}
	m_OwnedComp.clear();
}
