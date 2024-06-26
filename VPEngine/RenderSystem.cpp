#include "pch.h"
#include "RenderSystem.h"
#include "Components.h"
#include "EventManager.h"
#include "../VPGraphics/IGraphics.h"
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

		m_Graphics->AddRenderModel(meshComponent->FBXFilter, meshComponent->GetEntityID(), Name, Path);
		return;
	}

	//Skinned
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}

	//
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}


}

void RenderSystem::OnReleasedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}

	//Skinned
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}

	//
	if (comp->GetHandle()->type().id() == Reflection::GetTypeID<MeshComponent>())
	{
		MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);

		return;
	}

}

void RenderSystem::FixedUpdate(float deltaTime)
{
	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		//IGraphics::Getinstance().Render(uint32_t, transform, ~~정보);
		std::shared_ptr<RenderData> temp = std::make_shared<RenderData>(); 
		IDComponent* idComp = meshComp.GetComponent<IDComponent>();
		std::wstring FbxName{};
		std::wstring Name{};
		FbxName = FbxName.assign(meshComp.FBX.begin(), meshComp.FBX.end());
		Name = Name.assign(idComp->Name.begin(), idComp->Name.end());
		temp->EntityID = meshComp.GetEntityID();
		temp->FBX = FbxName;
		temp->Filter = meshComp.FBXFilter;
		temp->local = VPMath::Matrix::Identity;
		temp->world = meshComp.GetComponent<TransformComponent>()->WorldTransform;
		temp->Pass = PassState::Foward;
		temp->Name = Name;
		m_Graphics->UpdateModel(meshComp.GetEntityID(), temp);



	}

}


void RenderSystem::RenderUpdate(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		//IGraphics::Getinstance().Render(uint32_t, transform, ~~정보);
		std::shared_ptr<RenderData> temp=std::make_shared<RenderData>();
		std::wstring FbxName = FbxName.assign(meshComp.FBX.begin(), meshComp.FBX.end());
		temp->EntityID= meshComp.GetEntityID();
		temp->FBX= FbxName;
		temp->Filter= meshComp.FBXFilter;
		temp->local = VPMath::Matrix::Identity;
		temp->world= meshComp.GetComponent<TransformComponent>()->WorldTransform;
		temp->Pass= PassState::Foward ;

		m_Graphics->UpdateModel(meshComp.GetEntityID(), temp);



	}



}

