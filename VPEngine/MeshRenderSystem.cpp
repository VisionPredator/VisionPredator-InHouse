#include "pch.h"
#include "MeshRenderSystem.h"
#include "Components.h"
#include "EventManager.h"
MeshRenderSystem::MeshRenderSystem(SceneManager* sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnInitializeSystem", CreateSubscriber(&MeshRenderSystem::OnInitializeSystem));
	EventManager::GetInstance().Subscribe("OnAddedComponent", CreateSubscriber(&MeshRenderSystem::OnAddedComponent));
	EventManager::GetInstance().Subscribe("OnFinalizeSystem", CreateSubscriber(&MeshRenderSystem::OnFinalizeSystem));
	EventManager::GetInstance().Subscribe("OnInitializeEntity", CreateSubscriber(&MeshRenderSystem::OnInitializeEntity));
	EventManager::GetInstance().Subscribe("OnFinalizeEntity", CreateSubscriber(&MeshRenderSystem::OnFinalizeEntity));

}
void MeshRenderSystem::OnInitializeSystem(std::any)
{
	for (MeshComponent& meshComp : CompIter<MeshComponent>(m_SceneManager))
	{
		uint32_t entityID = meshComp.GetEntityID();
	}

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		uint32_t entityID = meshComp.GetEntityID();
	}
}

void MeshRenderSystem::OnInitializeEntity(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);
}

void MeshRenderSystem::OnFinalizeEntity(std::any data)
{
	uint32_t entityID = std::any_cast<uint32_t>(data);

}



void MeshRenderSystem::OnFinalizeSystem(std::any)
{
	///TODO: 유승운 MeshRenderor 삭제함수!
}

void MeshRenderSystem::OnAddedComponent(std::any data)
{
	auto comp = std::any_cast<Component*>(data);
	if (comp->GetTypeID() != Reflection::GetTypeID<MeshComponent>())
		return;
	MeshComponent* meshComponent = static_cast<MeshComponent*>(comp);
	// Graphic에 등록할 MeshEntity
	//IGraphics::Getinstance().AddEntity(uint32_t, 정보);
}

void MeshRenderSystem::FixedUpdate(float deltaTime)
{
	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{

	}
}


void MeshRenderSystem::RenderUpdate(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		//IGraphics::Getinstance().Render(uint32_t, transform, ~~정보);

	}
}
