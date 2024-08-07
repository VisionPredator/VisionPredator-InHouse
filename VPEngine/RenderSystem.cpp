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

		///TODO=========Name 은 딱히 필요없을 것 같으니 제거 생각중===========
		auto IDComp = meshComponent->GetComponent<IDComponent>();
		std::wstring Name{};
		Name.assign(IDComp->Name.begin(), IDComp->Name.end());
		/// FBX도 RenderData에 있으므로 보내주지 않아도 될듯함.
		std::wstring Path= meshComponent->FBX;
		///================================================================
		///Graphic에 RenderModel 만들 때 std::share_ptr<RenderData> 받기
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->FBX = meshComponent->FBX;
		meshComponent->Renderdata->Filter = meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		
		///인터페이스 수정해주세요!!+ RenderData 필요없는 데이터 정리 필요! 
		/// EntityID Name 정보는 필요없을 듯합니다. 어차피 unordered_Map<uint32t >로 연결하고있으니.
		/// m_Graphics->AddRenderModel(uint32_t, std::shared_ptr<RenderData>) 형식의 인터페이스!
		/// m_Graphics->AddRenderModel(meshComponent->GetEntityID(),meshComponent->Renderdata)
		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID(), Path);
		return;
	}
	// Skinned Mesh
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		const TransformComponent& Transform = *meshComponent->GetComponent<TransformComponent>();
		///TODO=========Name 은 딱히 필요없을 것 같으니 제거 생각중===========
		auto IDComp = meshComponent->GetComponent<IDComponent>();
		std::wstring Name{};
		Name.assign(IDComp->Name.begin(), IDComp->Name.end());
		/// FBX도 RenderData에 있으므로 보내주지 않아도 될듯함.
		std::wstring Path= meshComponent->FBX;
		///================================================================
		///Graphic에 RenderModel 만들 때 std::share_ptr<RenderData> 받기
		///보내주기전에 Initialize();
		meshComponent->Renderdata = std::make_shared<RenderData>();
		meshComponent->Renderdata->FBX= meshComponent->FBX;
		meshComponent->Renderdata->Filter= meshComponent->FBXFilter;
		meshComponent->Renderdata->world = Transform.WorldTransform;
		meshComponent->Renderdata->duration = 0;
		///인터페이스 수정해주세요!!+ RenderData 필요없는 데이터 정리 필요! 
		/// EntityID Name 정보는 필요없을 듯합니다. 어차피 unordered_Map<uint32t >로 연결하고있으니.
		/// m_Graphics->AddRenderModel(uint32_t, std::shared_ptr<RenderData>) 형식의 인터페이스!
		/// m_Graphics->AddRenderModel(meshComponent->GetEntityID(),meshComponent->Renderdata)
		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID(), Path);
		return;
	}

	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<GeometryComponent>())
	{
		GeometryComponent* meshComponent = static_cast<GeometryComponent*>(comp);
		auto IDComp = meshComponent->GetComponent<IDComponent>();
		meshComponent->Renderdata = std::make_shared<RenderData>();
		///마찬가지로 RenderData initialize 이후 다음 함수로 보내주세요!
		/// m_Graphics->AddRenderModel(meshComponent->GetEntityID(),meshComponent->Renderdata)

		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID());
		return;
	}	// Particle Object
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<ParticleComponent>())
	{
		ParticleComponent* component = static_cast<ParticleComponent*>(comp);
		std::string Path;
		Path.assign(component->TexturePath.begin(), component->TexturePath.end());

		effect::ParticleInfo info;
		info.TexturePath = Path;
		m_Graphics->CreateParticleObject(component->GetEntityID(), info);
		return;
	}}

void RenderSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());
		meshComponent->Renderdata.reset();
		return;
	}

	//Skinned
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
}

void RenderSystem::MeshCompRender(MeshComponent& meshComp)
{
	const TransformComponent& transform = *meshComp.GetComponent<TransformComponent>();
	bool IsChanged=false;
	auto renderdata = meshComp.Renderdata;

	renderdata->FBX = meshComp.FBX;
	renderdata->world = transform.WorldTransform;
	renderdata->Filter = meshComp.FBXFilter;
	///m_Graphics->UpdateModel(meshComp.GetEntityID()); RenderData를 찾아서 Pass 관리만 해주면 될듯?

	///TODO============구형 버전 삭제될 예정================
	//IGraphics::Getinstance().Render(uint32_t, transform, ~~정보);
	///RenderData  연산자 = 필요합니당~
	RenderData temp; 
	temp.EntityID = meshComp.GetEntityID();
	// Use move semantics to avoid unnecessary copying
	temp.FBX = meshComp.FBX;
	auto idComp = meshComp.GetComponent<IDComponent>();
	temp.Name = std::wstring(idComp->Name.begin(), idComp->Name.end());
	temp.Filter = meshComp.FBXFilter;
	temp.world = meshComp.GetComponent<TransformComponent>()->WorldTransform;
	temp.duration = 0;
	m_Graphics->UpdateModel(meshComp.GetEntityID(), temp);
	///====================================================
}

void RenderSystem::SkincompRender(SkinningMeshComponent& skinComp)
{
	const TransformComponent& transform = *skinComp.GetComponent<TransformComponent>();
	auto renderdata = skinComp.Renderdata;
	renderdata->FBX = skinComp.FBX;
	renderdata->world = transform.WorldTransform;
	renderdata->Filter = skinComp.FBXFilter;
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
	///m_Graphics->UpdateModel(meshComp.GetEntityID()); 이하동문

	///TODO============구형 버전 삭제될 예정================
	auto idComp = skinComp.GetComponent<IDComponent>();
	RenderData temp;
	temp.EntityID = idComp->GetEntityID();
	std::wstring FbxName{};
	std::wstring Name{};
	FbxName = FbxName.assign(skinComp.FBX.begin(), skinComp.FBX.end());
	Name = Name.assign(idComp->Name.begin(), idComp->Name.end());
	temp.Filter = skinComp.FBXFilter;
	temp.world = skinComp.GetComponent<TransformComponent>()->WorldTransform;
	temp.Name = Name;

	temp.FBX = FbxName;
	temp.duration = 0;

	if (skinComp.HasComponent<AnimationComponent>())
	{
		auto anicomp = skinComp.GetComponent<AnimationComponent>();

		temp.curAnimation = anicomp->curAnimation;
		if (!temp.curAnimation.empty())
		{
			temp.FBX = temp.curAnimation;
			temp.duration = anicomp->duration;
			temp.isPlay = anicomp->isPlay;
			temp.isChange = anicomp->isChange;
			temp.preDuration = anicomp->preDuration;
			temp.preAnimation = anicomp->preAnimation;
		}
	}

	m_Graphics->UpdateModel(skinComp.GetEntityID(), temp);
	///====================================================

}

void RenderSystem::GeometryRender(GeometryComponent& geometryComp)
{
	const TransformComponent& transform = *geometryComp.GetComponent<TransformComponent>();

	auto renderdata = geometryComp.Renderdata;
	renderdata->color = geometryComp.color;
	renderdata->useTexture = geometryComp.UseTexture;
	renderdata->Pass = geometryComp.pass;
	renderdata->Filter = geometryComp.FBXFilter;
	renderdata->world = transform.WorldTransform;
	renderdata->useTexture = geometryComp.UseTexture;
	renderdata->textureName = geometryComp.TextureName;
	///m_Graphics->UpdateModel(meshComp.GetEntityID()); 이하동문

	m_Graphics->UpdateModel(geometryComp.GetEntityID(), *renderdata.get());

}


