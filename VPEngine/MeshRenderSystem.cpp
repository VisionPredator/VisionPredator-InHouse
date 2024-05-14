#include "pch.h"
#include "Components.h"
#include "MeshRenderSystem.h"

void MeshRenderSystem::OnInitializeSystem(std::any)
{
	///TODO: 유승운 MeshRenderor등록함수!

	for (MeshComponent& meshComp : CompIter<MeshComponent>(m_EntityManager))
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
