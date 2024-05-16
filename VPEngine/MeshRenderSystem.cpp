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
	///TODO: ���¿� MeshRenderor����Լ�!

	for (MeshComponent& meshComp : CompIter<MeshComponent>(m_SceneManager))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///���⼭ Entity ID �� �ش� �������� �����ֱ�!
	}

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///���⼭ Entity ID �� �ش� �������� �����ֱ�!
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
	///TODO: ���¿� MeshRenderor �����Լ�!


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
