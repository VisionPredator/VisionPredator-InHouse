#include "pch.h"
#include "InterectiveSystem.h"
#include "VisPredComponents.h"
#include "EventManager.h"
InterectiveSystem::InterectiveSystem(std::shared_ptr<SceneManager> scenemanager):System(scenemanager)
{
	EventManager::GetInstance().Subscribe("OnSearched", CreateSubscriber(&InterectiveSystem::OnSearched));
	EventManager::GetInstance().Subscribe("OnUnSearched", CreateSubscriber(&InterectiveSystem::OnUnSearched));
}

void InterectiveSystem::OnSearched(std::any searchedEntityID)
{
	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;

	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();
	if (!interective->IsInterective)
		return;
	if (interectedEntity->HasComponent<MeshComponent>())
	{
		auto meshcomp = interectedEntity->GetComponent<MeshComponent>();
		meshcomp->MaskColor = interective->SearcehdColor;
	}
}

void InterectiveSystem::OnUnSearched(std::any searchedEntityID)
{

	auto searchedID = std::any_cast<uint32_t>(searchedEntityID);
	if (!GetSceneManager()->HasComponent<InterectiveComponent>(searchedID))
		return;

	auto interectedEntity = GetSceneManager()->GetEntity(searchedID);
	auto interective = interectedEntity->GetComponent<InterectiveComponent>();
	if (!interective->IsInterective)
		return;

	if (interectedEntity->HasComponent<MeshComponent>())
	{
		auto meshcomp = interectedEntity->GetComponent<MeshComponent>();
		meshcomp->MaskColor = {};
	}

}

void InterectiveSystem::OnInterective(std::any interective_interector)
{
	auto [interect, interecive] = std::any_cast<std::pair<uint32_t, uint32_t>>(interective_interector);



}
