#include "pch.h"
#include "CabinetSystem.h"
#include "VisPredComponents.h"
CabinetSystem::CabinetSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&CabinetSystem::OnInterected));

}

void CabinetSystem::FixedUpdate(float deltaTime)
{

	COMPLOOP(CabinetComponent, cabinetcomp)
	{
		if (!cabinetcomp.IsOpen)
			continue;

		cabinetcomp.OpenProgress += deltaTime;
		if (cabinetcomp.OpenProgress > cabinetcomp.OpenTime)
		{
			if (!cabinetcomp.RighEntity || !cabinetcomp.LeftEntity)
				continue;
			auto rightTrans = cabinetcomp.RighEntity->GetComponent<TransformComponent>();
			rightTrans->SetLocalRotation(VPMath::Vector3::Lerp(cabinetcomp.RightStartRotate, cabinetcomp.RightEndRotation, 1));

			auto leftTrans = cabinetcomp.LeftEntity->GetComponent<TransformComponent>();
			leftTrans->SetLocalRotation(VPMath::Vector3::Lerp(cabinetcomp.LeftStartRotate, cabinetcomp.LeftEndRotation, 1));

			cabinetcomp.IsOpen = false;
		}

		if (!cabinetcomp.RighEntity || !cabinetcomp.LeftEntity)
			continue;
		auto rightTrans = cabinetcomp.RighEntity->GetComponent<TransformComponent>();
		rightTrans->SetLocalRotation(VPMath::Vector3::Lerp(cabinetcomp.RightStartRotate, cabinetcomp.RightEndRotation, (cabinetcomp.OpenProgress / cabinetcomp.OpenTime)));

		auto leftTrans = cabinetcomp.LeftEntity->GetComponent<TransformComponent>();
		leftTrans->SetLocalRotation(VPMath::Vector3::Lerp(cabinetcomp.LeftStartRotate, cabinetcomp.LeftEndRotation, (cabinetcomp.OpenProgress / cabinetcomp.OpenTime)));
	}


}

void CabinetSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);
	if (!interected->HasComponent<CabinetComponent>())
		return;

	interected->GetComponent<InterectiveComponent>()->IsInterective = false;
	auto cabinet = interected->GetComponent<CabinetComponent>();
	cabinet->IsOpen = true;
}

void CabinetSystem::Initialize()
{
	COMPLOOP(CabinetComponent, boxcomp)
		Start(boxcomp.GetEntityID());
}

void CabinetSystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->GetEntity(gameObjectId)->HasComponent<CabinetComponent>())
		return;
	auto cabinetcomp = GetSceneManager()->GetEntity(gameObjectId)->GetComponent<CabinetComponent>();
	auto rightEntity = GetSceneManager()->GetChildEntityByName(gameObjectId, cabinetcomp->RightDoor);
	auto leftEntity = GetSceneManager()->GetChildEntityByName(gameObjectId, cabinetcomp->LeftDoor);
	if (!rightEntity || !leftEntity)
		return;
	cabinetcomp->RighEntity = rightEntity;
	cabinetcomp->LeftEntity = leftEntity;
	cabinetcomp->RightStartRotate = cabinetcomp->RighEntity->GetComponent<TransformComponent>()->Local_Rotation;
	cabinetcomp->LeftStartRotate = cabinetcomp->LeftEntity->GetComponent<TransformComponent>()->Local_Rotation;
	cabinetcomp->RightEndRotation.y -= cabinetcomp->OpenAngle;
	cabinetcomp->LeftEndRotation.y += cabinetcomp->OpenAngle;
}


void CabinetSystem::Finish(uint32_t gameObjectId)
{
}

void CabinetSystem::Finalize()
{
}
