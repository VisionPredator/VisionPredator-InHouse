#include "pch.h"
#include "Components.h"
#include "MeshRenderSystem.h"

void MeshRenderSystem::OnInitializeSystem(std::any)
{
	///TODO: ���¿� MeshRenderor����Լ�!

	for (MeshComponent& meshComp : CompIter<MeshComponent>(m_EntityManager))
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
