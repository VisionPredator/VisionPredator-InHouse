#include "pch.h"
#include "SocketSystem.h"

SocketSystem::SocketSystem(std::shared_ptr<SceneManager> sceneManager): System { sceneManager }
{
}

void SocketSystem::Update(float deltaTime)
{

}

void SocketSystem::BeginRenderUpdate(float deltaTime)
{

}

void SocketSystem::EditorRenderUpdate(float deltaTime)
{
}

void SocketSystem::RenderUpdate(float deltaTime)
{
	COMPLOOP(SocketComponent, socketcomp)
	{
		if (!socketcomp.IsConneted)
			continue;
		if (socketcomp.PreviewConnectedEntity != socketcomp.ConnectedEntity)
		{
			socketcomp.PreviewConnectedEntity = socketcomp.ConnectedEntity;
			auto tempentity = GetSceneManager()->GetEntityByIdentityName(socketcomp.ConnectedEntity);
			if (!tempentity)
			{
				socketcomp.ConnectedEntityID = 0;
				continue;
			}
			socketcomp.ConnectedEntityID = GetSceneManager()->GetEntityByIdentityName(socketcomp.ConnectedEntity)->GetEntityID();
		}
		auto entity = GetSceneManager()->GetEntity(socketcomp.ConnectedEntityID);
		if (!entity || !entity->HasComponent<SkinningMeshComponent>())
			continue;

		VPMath::Matrix matrix = m_Graphics->Attachment(entity->GetEntityID(), socketcomp.SocketName);
		VPMath::Quaternion tempQ = VPMath::Quaternion::CreateFromYawPitchRoll(
			VPMath::XMConvertToRadians(socketcomp.Rotation.y),
			VPMath::XMConvertToRadians(socketcomp.Rotation.x),
			VPMath::XMConvertToRadians(socketcomp.Rotation.z));


		///로컬 매트릭스 만들기
		VPMath::Matrix offsetMat = VPMath::Matrix::CreateFromQuaternion(tempQ) * VPMath::Matrix::CreateTranslation(socketcomp.Offset);
		auto Finalpose = offsetMat * matrix;

		VPMath::Vector3 tempscale{};
		VPMath::Quaternion tempQuater{};
		VPMath::Vector3 tempsworld{};

		Finalpose.Decompose(tempscale, tempQuater, tempsworld);

		TransformComponent* temptrnasform = socketcomp.GetComponent<TransformComponent>();
		temptrnasform->SetWorldLocation(tempsworld);
		temptrnasform->SetWorldQuaternion(tempQuater);
		temptrnasform->SetWorldScale(tempscale);
	}

	EventManager::GetInstance().ImmediateEvent("OnUpdate");

}

void SocketSystem::LateRenderUpdate(float deltaTime)
{
	COMPLOOP(SocketComponent, socketcomp)
	{

		UpdateSocketRenderData(*socketcomp.GetComponent<TransformComponent>());
		//std::list<uint32_t> UpdateListID;

		//UpdateListID.push_back(socketcomp.GetEntityID());

		//if (socketcomp.HasComponent<Children>())
		//{
		//	auto childcomp = socketcomp.GetComponent<Children>();
		//	for (auto childID : childcomp->ChildrenID)
		//	{
		//		UpdateListID.push_back(childID);
		//	}
		//}

		//if (!socketcomp.HasComponent<MeshComponent>())
		//	return;
		//auto& meshComp = *socketcomp.GetComponent<MeshComponent>();
		//const TransformComponent& transform = *socketcomp.GetComponent<TransformComponent>();
		//bool IsChanged = false;
		//auto renderdata = meshComp.Renderdata;

		//renderdata->FBX = meshComp.FBX;
		//renderdata->world = transform.WorldTransform;
		//renderdata->rotation = transform.World_Rotation;
		//renderdata->MaskingColor = meshComp.MaskColor;

	}

}

void SocketSystem::UpdateSocketRenderData(TransformComponent& transformcomp)
{
	if (transformcomp.HasComponent<MeshComponent>())
	{
		auto& meshComp = *transformcomp.GetComponent<MeshComponent>();
		bool IsChanged = false;
		auto renderdata = meshComp.Renderdata;

		renderdata->FBX = meshComp.FBX;
		renderdata->world = transformcomp.WorldTransform;
		renderdata->rotation = transformcomp.World_Rotation;
		renderdata->MaskingColor = meshComp.MaskColor;
	}

	if (transformcomp.HasComponent<Children>())
	{
		auto childcomp = transformcomp.GetComponent<Children>();
		for (auto entityid: childcomp->ChildrenID)
		{

			UpdateSocketRenderData(*GetSceneManager()->GetComponent<TransformComponent>(entityid));
		}

	}


}

