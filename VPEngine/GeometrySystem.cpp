#include "pch.h"
#include "GeometrySystem.h"

GeometrySystem::GeometrySystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void GeometrySystem::ComponentAdded(Component* comp)
{
	/*if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		auto IDComp = meshComponent->GetComponent<IDComponent>();
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();
		meshComponent->Renderdata->Filter = meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->punchEffect = meshComponent->UseTexture;
		meshComponent->Renderdata->color = meshComponent->color;
		meshComponent->Renderdata->textureName = meshComponent->TextureName;
		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}*/
}

void GeometrySystem::ComponentReleased(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();

		return;
	}
}

void GeometrySystem::BeginRenderUpdate(float deltaTime)
{
}

void GeometrySystem::RenderUpdate(float deltaTime)
{

}

void GeometrySystem::LateRenderUpdate(float deltaTime)
{
}

void GeometrySystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}
