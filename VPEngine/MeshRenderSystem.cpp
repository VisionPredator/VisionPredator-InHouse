#include "pch.h"
#include "Components.h"
#include "MeshRenderSystem.h"


MeshRenderSystem::MeshRenderSystem(EntityManager* entitymanager) :System(entitymanager)
{
	EventManager::GetInstance().Subscribe("OnInitializeSystem", CreateSubscriber(&MeshRenderSystem::OnInitializeSystem));
	EventManager::GetInstance().Subscribe("OnFinalizeSystem",CreateSubscriber(&MeshRenderSystem::OnFinalizeSystem));
}


void MeshRenderSystem::OnInitializeSystem(std::any data)	//데이터에는 아무 값도 안들어가게 될 것 같음.
{
	///TODO: 유승운 MeshRenderor등록함수!



	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///여기서 Entity ID 와 해당 정보정보 보내주기! 어떤 파일을 쓸건지.
		//grahics::Interface->AddMeshEntity(entityID, filepath, trasnform);
		
	}
}

void MeshRenderSystem::OnFinalizeSystem(std::any)
{
	///TODO: 유승운 MeshRenderor 삭제함수!
	//grahics::Interface->DeleteMeshEntity(entityID, filepath, trasnform);


}

void MeshRenderSystem::FixedUpdate(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		///TODO:: 유승운 Mesh별로 보내줄 함수 위치값 같은거??
		///TODO:: 유승운 혹시 도중에 mesh 형태를 바꿀 생각 있다면, 재설정 함수 있으면 좋으려나?
	}
}
