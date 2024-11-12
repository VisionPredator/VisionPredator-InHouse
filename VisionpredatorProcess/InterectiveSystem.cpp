#include "pch.h"
#include "InterectiveSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
#include <EngineStructs.h>
InterectiveSystem::InterectiveSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnSearched", CreateSubscriber(&InterectiveSystem::OnSearched));
	EventManager::GetInstance().Subscribe("OnUnSearched", CreateSubscriber(&InterectiveSystem::OnUnSearched));
	EventManager::GetInstance().Subscribe("OnInterective", CreateSubscriber(&InterectiveSystem::OnInterective));
	EventManager::GetInstance().Subscribe("OnInterected", CreateSubscriber(&InterectiveSystem::OnInterected));
}

void InterectiveSystem::OnSearched(std::any searchedEntityID)
{
	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	EventManager::GetInstance().ImmediateEvent("OnUpdateSearchUI");
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;
	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();
	if (!interective->IsInterective)
		return;

	auto meshcompvec = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(interectedEntity->GetEntityID());
	if (meshcompvec.empty())
		return;
	for (auto meshcomp : meshcompvec)
		meshcomp->MaskColor = interective->SearcehdColor;

}

void InterectiveSystem::OnUnSearched(std::any searchedEntityID)
{
	EventManager::GetInstance().ImmediateEvent("OnResetInterectionUI");
	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;

	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();

	auto meshcompvec = GetSceneManager()->GetChildEntityComps_HasComp<MeshComponent>(interectedEntity->GetEntityID());
	if (meshcompvec.empty())
		return;
	for (auto meshcomp : meshcompvec)
		meshcomp->MaskColor = {};

}

void InterectiveSystem::OnInterective(std::any interective_interector)
{
	auto [first, second] = std::any_cast<std::pair<uint32_t, uint32_t>>(interective_interector);
	auto firstentity = GetSceneManager()->GetEntity(first);
	auto secondentity = GetSceneManager()->GetEntity(second);
	if (!firstentity || !secondentity)
		return;
	if (firstentity->HasComponent<InterectiveComponent>())
	{
		if (!firstentity->GetComponent<InterectiveComponent>()->IsInterective)
			return;
		std::pair< std::shared_ptr<Entity>, std::shared_ptr<Entity > > temp = { firstentity,secondentity };
		std::any data = temp;
		EventManager::GetInstance().ImmediateEvent("OnInterected", data);
	}
	else if (secondentity->HasComponent<InterectiveComponent>())
	{
		if (!secondentity->GetComponent<InterectiveComponent>()->IsInterective)
			return;
		std::pair< std::shared_ptr<Entity>, std::shared_ptr<Entity > > temp = { secondentity, firstentity };
		std::any data = temp;
		EventManager::GetInstance().ImmediateEvent("OnInterected", data);
	}


}

void InterectiveSystem::OnInterected(std::any interective_interector)
{
	auto [interected, interector] = std::any_cast<std::pair<std::shared_ptr<Entity>, std::shared_ptr<Entity>>>(interective_interector);
	if (!interected->HasComponent<InterectiveComponent>())
		return;
	auto interectedcomp = interected->GetComponent<InterectiveComponent>();
	GetSceneManager()->SpawnSoundEntity(interectedcomp->Soundkey, interectedcomp->Volume,false, false, interected->GetComponent<TransformComponent>()->World_Location);
}
