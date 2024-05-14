#include "pch.h"
#include "Components.h"
#include "MeshRenderSystem.h"


MeshRenderSystem::MeshRenderSystem(EntityManager* entitymanager) :System(entitymanager)
{
	EventManager::GetInstance().Subscribe("OnInitializeSystem", CreateSubscriber(&MeshRenderSystem::OnInitializeSystem));
	EventManager::GetInstance().Subscribe("OnFinalizeSystem",CreateSubscriber(&MeshRenderSystem::OnFinalizeSystem));
}


void MeshRenderSystem::OnInitializeSystem(std::any data)	//�����Ϳ��� �ƹ� ���� �ȵ��� �� �� ����.
{
	///TODO: ���¿� MeshRenderor����Լ�!



	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		uint32_t entityID = meshComp.GetEntityID();
		///���⼭ Entity ID �� �ش� �������� �����ֱ�! � ������ ������.
		//grahics::Interface->AddMeshEntity(entityID, filepath, trasnform);
		
	}
}

void MeshRenderSystem::OnFinalizeSystem(std::any)
{
	///TODO: ���¿� MeshRenderor �����Լ�!
	//grahics::Interface->DeleteMeshEntity(entityID, filepath, trasnform);


}

void MeshRenderSystem::FixedUpdate(float deltaTime)
{

	for (MeshComponent& meshComp : COMPITER(MeshComponent))
	{
		///TODO:: ���¿� Mesh���� ������ �Լ� ��ġ�� ������??
		///TODO:: ���¿� Ȥ�� ���߿� mesh ���¸� �ٲ� ���� �ִٸ�, �缳�� �Լ� ������ ��������?
	}
}
