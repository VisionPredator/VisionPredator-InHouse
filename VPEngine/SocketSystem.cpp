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

void SocketSystem::TargetConnectedID(SocketComponent& socketcomp)
{
	if (socketcomp.PreviewConnectedEntity != socketcomp.ConnectedEntity)
	{
		socketcomp.PreviewConnectedEntity = socketcomp.ConnectedEntity;
		auto tempentity = GetSceneManager()->GetEntityByIdentityName(socketcomp.ConnectedEntity);
		if (!tempentity)
		{
			socketcomp.ConnectedEntityID = 0;
			socketcomp.PreConnectedEntityID = socketcomp.ConnectedEntityID;
		}
		else
		{
			socketcomp.ConnectedEntityID = GetSceneManager()->GetEntityByIdentityName(socketcomp.ConnectedEntity)->GetEntityID();
			socketcomp.PreConnectedEntityID = socketcomp.ConnectedEntityID;
		}

	}
	else if (socketcomp.PreConnectedEntityID != socketcomp.ConnectedEntityID)
	{
		socketcomp.PreConnectedEntityID = socketcomp.ConnectedEntityID;
		auto tempentity = GetSceneManager()->GetEntity(socketcomp.ConnectedEntityID);
		if (tempentity)
		{
		socketcomp.ConnectedEntity = tempentity->GetComponent<IDComponent>()->Name;
		socketcomp.PreviewConnectedEntity = socketcomp.ConnectedEntity;
		}
		else
		{
			socketcomp.ConnectedEntityID = 0;
			socketcomp.PreConnectedEntityID = 0;
		}
	}
}




void SocketSystem::RenderUpdate(float deltaTime)
{
	COMPLOOP(SocketComponent, socketcomp)
	{

		if (!socketcomp.IsConnected)
			continue;

		TargetConnectedID(socketcomp);

		auto entity = GetSceneManager()->GetEntity(socketcomp.ConnectedEntityID);
		if (!entity || !entity->HasComponent<SkinningMeshComponent>())
			continue;

		VPMath::Matrix attachmentMatrix = m_Graphics->Attachment(entity->GetEntityID(), socketcomp.SocketName);
		VPMath::Quaternion rotationQuat = /*socketcomp.offsetQuaternion;*/VPMath::Quaternion::CreateFromYawPitchRoll(
			VPMath::XMConvertToRadians(socketcomp.offsetQuaternion.y),
			VPMath::XMConvertToRadians(socketcomp.offsetQuaternion.x),
			VPMath::XMConvertToRadians(socketcomp.offsetQuaternion.z));


		///로컬 매트릭스 만들기
		VPMath::Matrix offsetMatrix = VPMath::Matrix::CreateTranslation(socketcomp.Offset);

		// 최종 매트릭스 계산 (회전을 먼저 적용한 후 오프셋 매트릭스를 곱함)
		VPMath::Matrix finalMatrix = VPMath::Matrix::CreateFromQuaternion(rotationQuat) * attachmentMatrix;
		finalMatrix = offsetMatrix * finalMatrix;

		VPMath::Vector3 tempscale{};
		VPMath::Quaternion tempQuater{};
		VPMath::Vector3 tempsworld{};

		finalMatrix.NewDecompose(tempscale, tempQuater, tempsworld);
		//if (!finalMatrix.Decompose(tempscale, tempQuater, tempsworld))
		//{
		//	finalMatrix.DecomposeWithFallback(tempscale, tempQuater, tempsworld);
		//}

		TransformComponent* temptrnasform = socketcomp.GetComponent<TransformComponent>();
		temptrnasform->SetWorldLocation(tempsworld);

		temptrnasform->SetWorldQuaternion(tempQuater);
		if (socketcomp.HasComponent<RigidBodyComponent>())
		{
			auto rigid = socketcomp.GetComponent<RigidBodyComponent>();
			if (rigid->IsDynamic)
				m_PhysicsEngine->SetVelocity(rigid->GetEntityID());
			VPMath::Vector3 speed = m_PhysicsEngine->GetVelocity(rigid->GetEntityID());
			speed = {};
		}
	}

	EventManager::GetInstance().ImmediateEvent("OnUpdate");

}

void SocketSystem::LateRenderUpdate(float deltaTime)
{
	COMPLOOP(SocketComponent, socketcomp)
	{
		UpdateSocketRenderData(*socketcomp.GetComponent<TransformComponent>());
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

