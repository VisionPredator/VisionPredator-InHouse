#include "pch.h"
#include "DecalSystem.h"

DecalSystem::DecalSystem(std::shared_ptr<SceneManager> sceneManager) : System(sceneManager)
{

}

void DecalSystem::BeginRenderUpdate(float deltaTime)
{
	for (DecalComponent& comp : COMPITER(DecalComponent))
	{
		auto transform = comp.GetComponent<TransformComponent>();

		decal::Info temp;
		temp.TexturePath = comp.TextureName;
		temp.WorldTransform = transform->WorldTransform;
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

		auto transform = comp->GetComponent<TransformComponent>();

		decal::Info temp;
		temp.TexturePath = component->TextureName;
		temp.WorldTransform = transform->WorldTransform;
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
