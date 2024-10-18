#include "pch.h"
#include "DecalSystem.h"

void DecalSystem::BeginRenderUpdate(float deltaTime)
{
	for (DecalComponent& comp : COMPITER(DecalComponent))
	{
		decal::Info temp;
		temp.TexturePath = comp.TextureName;
		temp.VolumeSize = comp.VolumeSize;
		m_Graphics->DrawDecal(temp);
	}
}

void DecalSystem::RenderUpdate(float deltaTime)
{

	
}

void DecalSystem::LateRenderUpdate(float deltaTime)
{
}

void DecalSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
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

void DecalSystem::Update(float deltaTime)
{
	

}
