#include "pch.h"
#include "MeshSystem.h"

MeshSystem::MeshSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void MeshSystem::ComponentAdded(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		///Graphic에 RenderModel 만들 때 std::share_ptr<RenderData> 받기
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();
		meshComponent->Renderdata->FBX = meshComponent->FBX;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->offset = meshComponent->LightMapOffset;
		meshComponent->Renderdata->lightmapindex = meshComponent->LightMapIndex;
		meshComponent->Renderdata->scale = meshComponent->LightMapScale;
		meshComponent->Renderdata->tiling = meshComponent->LightMapTiling;
		meshComponent->Renderdata->MaskingColor = meshComponent->MaskColor;
		meshComponent->Renderdata->color = meshComponent->InteractiveColor;

		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}
}

void MeshSystem::ComponentReleased(Component* comp)
{
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();
		return;
	}
}

void MeshSystem::BeginRenderUpdate(float deltaTime)
{
	COMPLOOP(MeshComponent, meshComp)
	{
		const TransformComponent& transform = *meshComp.GetComponent<TransformComponent>();
		bool IsChanged = false;
		auto renderdata = meshComp.Renderdata;
		renderdata->isVisible = meshComp.IsVisible;
		renderdata->isOverDraw = meshComp.IsOverDraw;
		renderdata->FBX = meshComp.FBX;
		renderdata->world = transform.WorldTransform;
		renderdata->rotation = transform.World_Rotation;
		renderdata->MaskingColor = meshComp.MaskColor;
		renderdata->tiling = meshComp.LightMapTiling;
		renderdata->offset = meshComp.LightMapOffset;
		renderdata->lightmapindex = meshComp.LightMapIndex;
		renderdata->color = meshComp.InteractiveColor;

	}
}

void MeshSystem::RenderUpdate(float deltaTime)
{



}

void MeshSystem::LateRenderUpdate(float deltaTime)
{

}

void MeshSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}
