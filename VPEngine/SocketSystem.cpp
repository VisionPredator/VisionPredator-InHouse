#include "pch.h"
#include "SocketSystem.h"

SocketSystem::SocketSystem(std::shared_ptr<SceneManager> sceneManager): System { sceneManager }
{
}

void SocketSystem::Update(float deltaTime)
{

}

void SocketSystem::RenderUpdate(float deltaTime)
{
	COMPLOOP(SocketComponent, socketcomp)
	{
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

void SocketSystem::EditorRenderUpdate(float deltaTime)
{
}
