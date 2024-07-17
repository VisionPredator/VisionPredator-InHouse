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

	//Skinned
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


}
void RenderSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());

	}

	//Skinned
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		SkinningMeshComponent* meshComponent = static_cast<SkinningMeshComponent*>(comp);
		m_Graphics->EraseObject(meshComponent->GetEntityID());

	}


	return;

}

void RenderSystem::RenderUpdate(float deltaTime)
{

	for (MeshComponent& comp : COMPITER(MeshComponent))
	{
		MeshCompRender(comp);
	}
	for (SkinningMeshComponent& comp : COMPITER(SkinningMeshComponent))
	{
		SkincompRender(comp);
	}
}

void RenderSystem::MeshCompRender(MeshComponent& comp)
{

	//IGraphics::Getinstance().Render(uint32_t, transform, ~~Á¤º¸);
	std::shared_ptr<RenderData> temp = std::make_shared<RenderData>();
	IDComponent* idComp = comp.GetComponent<IDComponent>();
	temp->EntityID = idComp->GetEntityID();
	std::wstring FbxName{};
	std::wstring Name{};
	FbxName = FbxName.assign(comp.FBX.begin(), comp.FBX.end());
	Name = Name.assign(idComp->Name.begin(), idComp->Name.end());
	temp->Filter = comp.FBXFilter;
	temp->world = comp.GetComponent<TransformComponent>()->WorldTransform;
	temp->Name = Name;
	temp->FBX = FbxName;
	temp->duration = 0;

	m_Graphics->UpdateModel(comp.GetEntityID(), temp);
}

void RenderSystem::SkincompRender(SkinningMeshComponent& comp)
{
	std::shared_ptr<RenderData> temp = std::make_shared<RenderData>();
	IDComponent* idComp = comp.GetComponent<IDComponent>();
	temp->EntityID = idComp->GetEntityID();
	std::wstring FbxName{};
	std::wstring Name{};
	FbxName = FbxName.assign(comp.FBX.begin(), comp.FBX.end());
	Name = Name.assign(idComp->Name.begin(), idComp->Name.end());
	temp->Filter = comp.FBXFilter;
	temp->world = comp.GetComponent<TransformComponent>()->WorldTransform;
	temp->Name = Name;

	temp->FBX = FbxName;
	temp->duration = 0;

	if (comp.HasComponent<AnimationComponent>())
	{
		auto anicomp = comp.GetComponent<AnimationComponent>();

		temp->curAnimation = anicomp->curAnimation;
		if (!temp->curAnimation.empty())
		{
			temp->FBX = temp->curAnimation;
			temp->duration = anicomp->duration;
			temp->isPlay = anicomp->isPlay;
		}
	}
	m_Graphics->UpdateModel(comp.GetEntityID(), temp);
}
