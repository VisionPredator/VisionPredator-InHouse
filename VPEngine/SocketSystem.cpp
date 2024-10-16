#include "pch.h"
#include "SocketSystem.h"
#include <iostream>

SocketSystem::SocketSystem(std::shared_ptr<SceneManager> sceneManager): System { sceneManager }
{
	EventManager::GetInstance().Subscribe("OnSocketUpdate",CreateSubscriber(&SocketSystem::OnSocketUpdate));
}

void SocketSystem::Update(float deltaTime)
{

}

void SocketSystem::BeginRenderUpdate(float deltaTime)
{
}

void SocketSystem::EditorRenderUpdate(float deltaTime)
{
	RenderUpdate(deltaTime);

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

void SocketSystem::OnSocketUpdate(std::any id)
{
	uint32_t entityid = std::any_cast<uint32_t>(id);

	auto entity = GetSceneManager()->GetEntity(entityid);
	if (!entity)
		return;
	if (!entity->HasComponent<SocketComponent>())
		return;
	auto& socketcomp = *entity->GetComponent<SocketComponent>();

	socketcomp.AttachmentMatrix = m_Graphics->Attachment(entity->GetEntityID(), socketcomp.SocketName);
	VPMath::Quaternion rotationQuat{};
	if (socketcomp.UseQuaternion)
	{
		rotationQuat = socketcomp.OffsetQuaternion;
		socketcomp.OffsetRotation = socketcomp.OffsetQuaternion.ToEuler() * 180 / VPMath::XM_PI;
	}
	else
	{
		rotationQuat = VPMath::Quaternion::CreateFromYawPitchRoll(
			VPMath::XMConvertToRadians(socketcomp.OffsetRotation.y),
			VPMath::XMConvertToRadians(socketcomp.OffsetRotation.x),
			VPMath::XMConvertToRadians(socketcomp.OffsetRotation.z));
		socketcomp.OffsetQuaternion = rotationQuat;
	}


	///���� ��Ʈ���� �����
	VPMath::Matrix offsetMatrix = VPMath::Matrix::CreateTranslation(socketcomp.Offset);
	// ���� ��Ʈ���� ��� (ȸ���� ���� ������ �� ������ ��Ʈ������ ����)
	VPMath::Matrix finalMatrix = VPMath::Matrix::CreateFromQuaternion(rotationQuat) * offsetMatrix * socketcomp.AttachmentMatrix;
	//finalMatrix = offsetMatrix * finalMatrix;

	VPMath::Vector3 tempscale{};
	VPMath::Quaternion tempQuater{};
	VPMath::Vector3 tempsworld{};

	finalMatrix.NewDecompose(tempscale, tempQuater, tempsworld);
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

		socketcomp.AttachmentMatrix = m_Graphics->Attachment(entity->GetEntityID(), socketcomp.SocketName);
		VPMath::Quaternion rotationQuat{};
		if (socketcomp.UseQuaternion)
		{
			rotationQuat = socketcomp.OffsetQuaternion;
			socketcomp.OffsetRotation = socketcomp.OffsetQuaternion.ToEuler() * 180 / VPMath::XM_PI;
		}
		else
		{
			rotationQuat = VPMath::Quaternion::CreateFromYawPitchRoll(
				VPMath::XMConvertToRadians(socketcomp.OffsetRotation.y),
				VPMath::XMConvertToRadians(socketcomp.OffsetRotation.x),
				VPMath::XMConvertToRadians(socketcomp.OffsetRotation.z));
			socketcomp.OffsetQuaternion = rotationQuat;
		}


		///���� ��Ʈ���� �����
		VPMath::Matrix offsetMatrix = VPMath::Matrix::CreateTranslation(socketcomp.Offset);
		// ���� ��Ʈ���� ��� (ȸ���� ���� ������ �� ������ ��Ʈ������ ����)
		VPMath::Matrix finalMatrix = VPMath::Matrix::CreateFromQuaternion(rotationQuat)* offsetMatrix * socketcomp.AttachmentMatrix;
		//finalMatrix = offsetMatrix * finalMatrix;

		VPMath::Vector3 tempscale{};
		VPMath::Quaternion tempQuater{};
		VPMath::Vector3 tempsworld{};

		finalMatrix.NewDecompose(tempscale, tempQuater, tempsworld);
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

