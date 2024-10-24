#include "pch.h"
#include "SkinnedMeshSystem.h"

SkinnedMeshSystem::SkinnedMeshSystem(std::shared_ptr<SceneManager> sceneManager) :System{ sceneManager }
{
}

void SkinnedMeshSystem::ComponentAdded(Component* comp)
{
	// Skinned Mesh
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();

		meshComponent->Renderdata->FBX = meshComponent->FBX;
		//meshComponent->Renderdata->Filter= meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->duration = 0;
		meshComponent->Renderdata->isSkinned = true;

		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}
}

void SkinnedMeshSystem::ComponentReleased(Component* comp)
{
	// skeletal mesh object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();

		return;
	}
}

void SkinnedMeshSystem::BeginRenderUpdate(float deltaTime)
{
	COMPLOOP(SkinningMeshComponent, skinComp)
	{
		const TransformComponent& transform = *skinComp.GetComponent<TransformComponent>();
		auto renderdata = skinComp.Renderdata;
		renderdata->isVisible = skinComp.IsVisible;
		renderdata->isOverDraw = skinComp.IsOverDraw;

		renderdata->FBX = skinComp.FBX;
		renderdata->world = transform.WorldTransform;
		renderdata->duration = 0;
		if (skinComp.HasComponent<AnimationComponent>())
		{
			auto anicomp = skinComp.GetComponent<AnimationComponent>();
			anicomp->FBX = skinComp.FBX;
			renderdata->duration = anicomp->duration;
			renderdata->isPlay = anicomp->isPlay;
			renderdata->preDuration = anicomp->preDuration;
		}
	}
}

void SkinnedMeshSystem::RenderUpdate(float deltaTime)
{

}

void SkinnedMeshSystem::LateRenderUpdate(float deltaTime)
{
}

void SkinnedMeshSystem::EditorRenderUpdate(float deltaTime)
{
	BeginRenderUpdate(deltaTime);
}
