#include "pch.h"
#include "DecalSystem.h"

void DecalSystem::BeginRenderUpdate(float deltaTime)
{
}

void DecalSystem::RenderUpdate(float deltaTime)
{
}

void DecalSystem::LateRenderUpdate(float deltaTime)
{
}

void DecalSystem::EditorRenderUpdate(float deltaTime)
{
}

void DecalSystem::ComponentAdded(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<DecalComponent>())
	{
		DecalComponent* component = static_cast<DecalComponent*>(comp);

		decal::Info temp;
		temp.TexturePath = component->TextureName;
		temp.VolumeSize = component->VolumeSize;
		m_Graphics->DrawDecal(temp);
		return;
	}
}

void DecalSystem::ComponentReleased(Component* comp)
{
}
