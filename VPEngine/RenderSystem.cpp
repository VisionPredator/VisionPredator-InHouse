#include "pch.h"
#include "RenderSystem.h"
#include "Components.h"
#include "EventManager.h"
RenderSystem::RenderSystem(SceneManager* sceneManager)
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
		auto IDComp = meshComponent->GetComponent<IDComponent>();
		std::wstring Name{};
		std::wstring Path{};
		Name.assign(IDComp->Name.begin(), IDComp->Name.end());
		Path.assign(meshComponent->FBX.begin(), meshComponent->FBX.end());

		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID(), Path);
		return;
	}

	// Skinned Mesh
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		auto IDComp = meshComponent->GetComponent<IDComponent>();
		std::wstring Name{};
		std::wstring Path{};
		Name.assign(IDComp->Name.begin(), IDComp->Name.end());
		Path.assign(meshComponent->FBX.begin(), meshComponent->FBX.end());

		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID(), Path);

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

}

void RenderSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());

		return;
	}

	//Skinned
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<SkinningMeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());

		return;
	}

	//
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}

}



void RenderSystem::RenderUpdate(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		MeshCompRender(meshComp);
	}

	for (SkinningMeshComponent& skinComp : COMPITER(SkinningMeshComponent))
	{
		SkincompRender(skinComp);
	}

	for (ParticleComponent& component : COMPITER(ParticleComponent))
	{
		effect::ParticleInfo info;
		info.TexturePath = component.TexturePath;

		m_Graphics->UpdateParticleObject(component.GetComponent<IDComponent>()->GetEntityID(), info);
	}
}

void RenderSystem::MeshCompRender(MeshComponent& meshComp)
{
	//IGraphics::Getinstance().Render(uint32_t, transform, ~~Á¤º¸);
	RenderData temp;
	IDComponent* idComp = meshComp.GetComponent<IDComponent>();
	temp.EntityID = idComp->GetEntityID();
	std::wstring FbxName{};
	std::wstring Name{};
	FbxName = FbxName.assign(meshComp.FBX.begin(), meshComp.FBX.end());
	Name = Name.assign(idComp->Name.begin(), idComp->Name.end());
	temp.Filter = meshComp.FBXFilter;
	temp.world = meshComp.GetComponent<TransformComponent>()->WorldTransform;
	temp.Name = Name;
	temp.FBX = FbxName;
	temp.duration = 0;

	m_Graphics->UpdateModel(meshComp.GetEntityID(), temp);
}

void RenderSystem::SkincompRender(SkinningMeshComponent& skinComp)
{
	IDComponent* idComp = skinComp.GetComponent<IDComponent>();
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
}
