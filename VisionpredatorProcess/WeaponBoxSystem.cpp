#include "pch.h"
#include "WeaponBoxSystem.h"
#include "EventManager.h"
#include "VisPredComponents.h"
WeaponBoxSystem::WeaponBoxSystem(std::shared_ptr<SceneManager> scenemanager) :System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&WeaponBoxSystem::OnInterected));
}

void WeaponBoxSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);
	if (!interected->HasComponent<WeaponBoxComponent>())
		return;

	interected->GetComponent<InterectiveComponent>()->IsInterective = false;
	auto weaponboxcomp = interected->GetComponent<WeaponBoxComponent>();
	weaponboxcomp->IsOpen = true;
}

void WeaponBoxSystem::FixedUpdate(float deltaTime)
{
	COMPLOOP(WeaponBoxComponent, boxcomp)
	{
		if (!boxcomp.IsOpen)
			continue;
	
		boxcomp.OpenProgress += deltaTime;
		if (boxcomp.OpenProgress > boxcomp.OpenTime)
		{
			if (!boxcomp.TopEntity)
				continue;
			auto toptrans = boxcomp.TopEntity->GetComponent<TransformComponent>();
			toptrans->SetLocalRotation(VPMath::Vector3::Lerp(boxcomp.StartRotation, boxcomp.EndRotation, 1));
			boxcomp.IsOpen = false;
			
			if (boxcomp.SpawnPrefabs.empty())
				continue;
			int rand_int = VPMath::Random_int(0, (boxcomp.SpawnPrefabs.size() - 1));
			auto weapon = GetSceneManager()->SpawnEditablePrefab(boxcomp.SpawnPrefabs[rand_int], boxcomp.SpawnPos, VPMath::Vector3{}, {});
			EventManager::GetInstance().ScheduleEvent("OnAddVelecity", std::make_tuple(weapon->GetEntityID(), (VPMath::Vector3{ 0.f,1.f,0.f } + (boxcomp.GetComponent<TransformComponent>()->FrontVector / 3)), boxcomp.SpawnSpeed));
			continue;
		}
		if (!boxcomp.TopEntity)
			continue;
		auto toptrans = boxcomp.TopEntity->GetComponent<TransformComponent>();
		toptrans->SetLocalRotation(VPMath::Vector3::Lerp(boxcomp.StartRotation, boxcomp.EndRotation, (boxcomp.OpenProgress / boxcomp.OpenTime)));
	}
}

void WeaponBoxSystem::Initialize()
{
	COMPLOOP(WeaponBoxComponent, boxcomp)
	{
		Start(boxcomp.GetEntityID());

	}
}

void WeaponBoxSystem::Start(uint32_t gameObjectId)
{
	if (!GetSceneManager()->GetEntity(gameObjectId)->HasComponent<WeaponBoxComponent>())
		return;
	auto weaponboxcomp = GetSceneManager()->GetEntity(gameObjectId)->GetComponent<WeaponBoxComponent>();
	auto topentity = GetSceneManager()->GetChildEntityByName(weaponboxcomp->GetEntityID(), weaponboxcomp->TopMesh);
	if (!topentity)
		return;
	weaponboxcomp->TopEntity = topentity;
	auto transform = topentity->GetComponent < TransformComponent>();
	weaponboxcomp->StartRotation = transform->Local_Location;
	weaponboxcomp->EndRotation = weaponboxcomp->StartRotation;
	weaponboxcomp->EndRotation.x -= weaponboxcomp->OpenAngle;
	auto trans = weaponboxcomp->GetComponent<TransformComponent>();
	weaponboxcomp->SpawnPos = trans->World_Location + weaponboxcomp->SpawnOffset;
}

void WeaponBoxSystem::Finish(uint32_t gameObjectId)
{
}

void WeaponBoxSystem::Finalize()
{
}
