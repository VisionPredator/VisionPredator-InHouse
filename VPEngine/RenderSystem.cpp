#include "pch.h"
#include "RenderSystem.h"
#include "Components.h"
#include "EventManager.h"
RenderSystem::RenderSystem(std::shared_ptr<SceneManager> sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&RenderSystem::OnAddedComponent));
	EventManager::GetInstance().Subscribe("OnReleasedComponent", CreateSubscriber(&RenderSystem::OnReleasedComponent));

}


void RenderSystem::OnAddedComponent(std::any data)
{
	// Static Mesh
	auto comp = std::any_cast<Component*>(data);

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
		meshComponent->Renderdata->Pass = meshComponent->Pass;

		///인터페이스 수정해주세요!!+ RenderData 필요없는 데이터 정리 필요! 
		/// EntityID Name 정보는 필요없을 듯합니다. 어차피 unordered_Map<uint32t >로 연결하고있으니.
		/// m_Graphics->AddRenderModel(uint32_t, std::shared_ptr<RenderData>) 형식의 인터페이스!
		/// m_Graphics->AddRenderModel(meshComponent->GetEntityID(),meshComponent->Renderdata)
		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}
	// Skinned Mesh
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();

		meshComponent->Renderdata->FBX= meshComponent->FBX;
		//meshComponent->Renderdata->Filter= meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->duration = 0;
		meshComponent->Renderdata->isSkinned = true;

		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}

	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();

		auto IDComp = meshComponent->GetComponent<IDComponent>();
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->EntityID = meshComponent->GetEntityID();
		meshComponent->Renderdata->Filter = meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->useTexture = meshComponent->UseTexture;
		meshComponent->Renderdata->color = meshComponent->color;
		meshComponent->Renderdata->textureName = meshComponent->TextureName;
		m_Graphics->AddRenderModel(meshComponent->Renderdata);
		return;
	}

	// Particle Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		std::string Path;
		Path.assign(component->TexturePath.begin(), component->TexturePath.end());

		effect::ParticleInfo info;
		info.TexturePath = Path;
		m_Graphics->CreateParticleObject(component->GetEntityID(), info);
		return;
	}

	// UI Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Sprite2DComponent>())
	{
		Sprite2DComponent* component = static_cast<Sprite2DComponent*>(comp);
		ui::ImageInfo info;
		info.ImagePath = component->TexturePath;
		info.PosXPercent = component->PosXPercent;
		info.PosYPercent = component->PosYPercent;
		info.Layer = component->Layer;
		info.Color = component->Color;
		info.Scale = component->Scale;

		m_Graphics->CreateImageObject(component->GetEntityID(), info);
	}
}

void RenderSystem::OnReleasedComponent(std::any data)
{
	// static mesh object
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();
		return;
	}

	// skeletal mesh object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();

		return;
	}

	//
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();

		return;
	}

	// particle object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		m_Graphics->DeleteParticleObjectByID(component->GetEntityID());
	}

	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<Sprite2DComponent>())
	{
		Sprite2DComponent* component = static_cast<Sprite2DComponent*>(comp);
		m_Graphics->DeleteImageObject(component->GetEntityID());
	}
}


void RenderSystem::RenderUpdate(float deltaTime)
{
	//THREAD_COMPONENTS(RenderSystem, MeshComponent, &RenderSystem::MeshCompRender);
	for (MeshComponent& meshcomp : COMPITER(MeshComponent))
	{
		MeshCompRender(meshcomp);
	}

	for (SkinningMeshComponent& skinComp : COMPITER(SkinningMeshComponent))
	{
		SkincompRender(skinComp);
	}

	for (GeometryComponent& geometryComp : COMPITER(GeometryComponent))
	{
		GeometryRender(geometryComp);
	}

	for (ParticleComponent& component : COMPITER(ParticleComponent))
	{
		effect::ParticleInfo info;
		info.TexturePath = component.TexturePath;
		info.MaxParticles = component.MaxParticle;

		m_Graphics->UpdateParticleObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}

	for (Sprite2DComponent& component : COMPITER(Sprite2DComponent))
	{
		ui::ImageInfo info;
		info.ImagePath = component.TexturePath;
		info.PosXPercent = component.PosXPercent;
		info.PosYPercent = component.PosYPercent;
		info.Layer = component.Layer;
		info.Color = component.Color;
		info.Scale = component.Scale;

		m_Graphics->UpdateImageObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}
}

void RenderSystem::MeshCompRender(MeshComponent& meshComp)
{
	const TransformComponent& transform = *meshComp.GetComponent<TransformComponent>();
	bool IsChanged=false;
	auto renderdata = meshComp.Renderdata;

	renderdata->FBX = meshComp.FBX;
	renderdata->world = transform.WorldTransform;
	renderdata->Pass = meshComp.Pass;
	renderdata->rotation = transform.World_Rotation;

}

void RenderSystem::SkincompRender(SkinningMeshComponent& skinComp)
{
	const TransformComponent& transform = *skinComp.GetComponent<TransformComponent>();
	auto renderdata = skinComp.Renderdata;
	renderdata->FBX = skinComp.FBX;
	renderdata->world = transform.WorldTransform;
	renderdata->duration = 0;
	if (skinComp.HasComponent<AnimationComponent>())
	{
		auto anicomp = skinComp.GetComponent<AnimationComponent>();

		renderdata->curAnimation = anicomp->curAnimation;
		if (!renderdata->curAnimation.empty())
		{
			renderdata->FBX = renderdata->curAnimation;
			renderdata->duration = anicomp->duration;
			renderdata->isPlay = anicomp->isPlay;
			renderdata->isChange = anicomp->isChange;
			renderdata->preDuration = anicomp->preDuration;
			renderdata->preAnimation = anicomp->preAnimation;
		}
	}
}

void RenderSystem::GeometryRender(GeometryComponent& geometryComp)
{
	const TransformComponent& transform = *geometryComp.GetComponent<TransformComponent>();

	auto renderdata = geometryComp.Renderdata;
	renderdata->color = geometryComp.color;
	renderdata->color.w = geometryComp.UseTexture;	//shader에서 color의 w값으로 텍스처있는지 판단함
	renderdata->useTexture = geometryComp.UseTexture;
	renderdata->Pass = geometryComp.pass;
	renderdata->Filter = geometryComp.FBXFilter;
	renderdata->world = transform.WorldTransform;
	renderdata->useTexture = geometryComp.UseTexture;
	renderdata->textureName = geometryComp.TextureName;
}


