#include "pch.h"
#include "DoorSystem.h"
#include "VisPredComponents.h"
DoorSystem::DoorSystem(std::shared_ptr<SceneManager> scenemanger) :System(scenemanger)
{
	EventManager::GetInstance().Subscribe("OnChangeDoorUseable", CreateSubscriber(&DoorSystem::OnChangeDoorUseable));
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&DoorSystem::OnInterected));
}

void DoorSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(DoorComponent, doorcomp)
	{
		if (!doorcomp.IsActivated)
			return;

		doorcomp.OpenProgress += deltaTime;
		if (doorcomp.OpenProgress > doorcomp.OpenTime)
		{
			doorcomp.OpenProgress = doorcomp.OpenTime;
		}
		if (doorcomp.IsOpen)
		{
			auto lefttrans = doorcomp.LeftEntity->GetComponent<TransformComponent>();
			VPMath::Vector3 newleftPos = VPMath::Vector3::Lerp(doorcomp.LeftClosePos, doorcomp.LeftOpenPos, (doorcomp.OpenProgress / doorcomp.OpenTime));
			lefttrans->SetLocalLocation(newleftPos);

			auto righttrans = doorcomp.RightEntity->GetComponent<TransformComponent>();
			VPMath::Vector3 newRightPos = VPMath::Vector3::Lerp(doorcomp.RightClosePos, doorcomp.RightOpenPos, (doorcomp.OpenProgress / doorcomp.OpenTime));
			righttrans->SetLocalLocation(newRightPos);
		}
		else if (!doorcomp.IsOpen)
		{
			auto lefttrans = doorcomp.LeftEntity->GetComponent<TransformComponent>();
			VPMath::Vector3 newleftPos = VPMath::Vector3::Lerp(doorcomp.LeftOpenPos, doorcomp.LeftClosePos, (doorcomp.OpenProgress / doorcomp.OpenTime));
			lefttrans->SetLocalLocation(newleftPos);

			auto righttrans = doorcomp.RightEntity->GetComponent<TransformComponent>();
			VPMath::Vector3 newRightPos = VPMath::Vector3::Lerp(doorcomp.RightOpenPos, doorcomp.RightClosePos, (doorcomp.OpenProgress / doorcomp.OpenTime));
			righttrans->SetLocalLocation(newRightPos);
		}
		if (doorcomp.OpenProgress >= doorcomp.OpenTime)
		{
			doorcomp.OpenProgress = 0.f;
			doorcomp.IsActivated = false;
		}
	}

}

void DoorSystem::Initialize()
{
	COMPLOOP(DoorComponent, doorcomp)
		Start(doorcomp.GetEntityID());
}

void DoorSystem::Start(uint32_t gameObjectId)
{
	auto entity = GetSceneManager()->GetEntity(gameObjectId);
	if (!entity || !entity->HasComponent<DoorComponent>())
		return;
	auto doorcomp = entity->GetComponent<DoorComponent>();
	auto leftentity = GetSceneManager()->GetChildEntityByName(gameObjectId,doorcomp->LeftDoor);
	auto rightentity = GetSceneManager()->GetChildEntityByName(gameObjectId,doorcomp->RightDoor);
	if (!leftentity || !rightentity)
		Log::GetClientLogger()->error("Door not founded");
	doorcomp->LeftEntity = leftentity;
	doorcomp->RightEntity = rightentity;
	auto lefttrans = doorcomp->LeftEntity->GetComponent<TransformComponent>();
	auto righttrans = doorcomp->RightEntity->GetComponent<TransformComponent>();
		doorcomp->LeftClosePos = lefttrans->Local_Location;
		doorcomp->RightClosePos = righttrans->Local_Location;
		doorcomp->LeftOpenPos = doorcomp->LeftClosePos;
		doorcomp->LeftOpenPos.x -= doorcomp->MoveDistance;
		doorcomp->RightOpenPos = doorcomp->RightClosePos;
		doorcomp->RightOpenPos.x += doorcomp->MoveDistance;

	if (doorcomp->IsOpen)
	{
		lefttrans->SetLocalLocation(doorcomp->RightClosePos);
		righttrans->SetLocalLocation(doorcomp->LeftClosePos);
	}

	if (doorcomp->IsUseserble)
		doorcomp->GetComponent<MeshComponent>()->FBX = doorcomp->UseableFBX;
	else
		doorcomp->GetComponent<MeshComponent>()->FBX = doorcomp->UnUseableFBX;
}

void DoorSystem::Finish(uint32_t gameObjectId)
{
}

void DoorSystem::Finalize()
{
}

void DoorSystem::OnChangeDoorUseable(std::any entityid_bool)
{
	auto [entityid, useable] = std::any_cast<std::pair<uint32_t, bool>>(entityid_bool);

	auto doorEntity = GetSceneManager()->GetEntity(entityid);
	if (!doorEntity || !doorEntity->HasComponent<DoorComponent>())
		return;
	auto doorcomp = doorEntity->GetComponent<DoorComponent>();
	if (doorcomp->IsUseserble == useable)
		return;
	doorcomp->IsUseserble = useable;
	if (doorcomp->IsUseserble)
		doorcomp->GetComponent<MeshComponent>()->FBX = doorcomp->UseableFBX;
	else
		doorcomp->GetComponent<MeshComponent>()->FBX = doorcomp->UnUseableFBX;

	if (!doorcomp->IsUseserble&& doorcomp->IsOpen)
	{
	doorcomp->IsActivated = true;
	doorcomp->IsOpen = false;
	}
	
	auto compvec = GetSceneManager()->GetChildEntityComps_HasComp<DoorOpenerComponent>(doorcomp->GetEntityID());
	if (compvec.empty())
		return;

	for (auto dooropen : compvec)
	{
		dooropen->GetComponent<InterectiveComponent>()->IsInterective = doorcomp->IsUseserble;
	}
}

void DoorSystem::OnInterected(std::any interective_interector) {
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);

	// DoorComponent가 없으면 함수 종료
	if (!interected->HasComponent<DoorOpenerComponent>())
		return;

	auto doorOpencomp = interected->GetComponent<DoorOpenerComponent>();

	// DoorIdentitys 벡터가 비어있을 경우 예외 처리
	//if (doorOpencomp->Dummy.empty())
	//	return;
	//auto doorentity = GetSceneManager()->GetEntityByIdentityName(doorOpencomp->Dummy);
	auto doorcomp = GetSceneManager()->GetParentEntityComp_HasComp<DoorComponent>(doorOpencomp->GetEntityID());
	// doorentity가 유효하지 않거나 DoorComponent가 없을 경우, 현재 루프 스킵
	if (!doorcomp)
		return;
	//auto doorcomp = doorentity->GetComponent<DoorComponent>();
	// DoorComponent가 이미 활성화되어 있으면 함수 종료
	if (doorcomp->IsActivated)
		return;
	// 문 활성화 및 열림 상태 전환
	doorcomp->IsActivated = true;
	doorcomp->IsOpen = !doorcomp->IsOpen;
}


