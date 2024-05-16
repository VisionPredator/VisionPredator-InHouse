#include "pch.h"
#include "MeshRenderSystem.h"
#include "Components.h"
#include "EventManager.h"
MeshRenderSystem::MeshRenderSystem(SceneManager* sceneManager)
	: System(sceneManager)
{
	EventManager::GetInstance().Subscribe("OnInitializeSystem", CreateSubscriber(&MeshRenderSystem::OnInitializeSystem));
	EventManager::GetInstance().Subscribe("OnFinalizeSystem", CreateSubscriber(&MeshRenderSystem::OnFinalizeSystem));
	EventManager::GetInstance().Subscribe("OnInitializeEntity", CreateSubscriber(&MeshRenderSystem::OnInitializeEntity));
	EventManager::GetInstance().Subscribe("OnFinalizeEntity", CreateSubscriber(&MeshRenderSystem::OnFinalizeEntity));

}
void MeshRenderSystem::OnInitializeSystem(std::any)
{
	///TODO: 유승운 MeshRenderor등록함수!

	for (MeshComponent& meshComp : CompIter<MeshComponent>(m_SceneManager))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///여기서 Entity ID 와 해당 정보정보 보내주기!
	}

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///여기서 Entity ID 와 해당 정보정보 보내주기!
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

void MeshRenderSystem::FixedUpdate(float deltaTime)
{
	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
	}
}
void MeshRenderSystem::Render(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{

	}
}
